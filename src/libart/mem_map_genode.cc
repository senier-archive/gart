// Genode includes
#include <base/log.h>
#include <base/exception.h>
#include <util/string.h>
#include <region_map/region_map.h>

// Android includes
#include <android-base/stringprintf.h>
#include "base/allocator.h"
#include <base/bit_utils.h>
#include <base/globals.h>

// libc/stdc++ includes
#include <stdlib.h>
#include <cinttypes>
#include <sys/mman.h>

// GART includes
#include <gart/env.h>

// local includes
#include "mem_map.h"

#define assert(invariant, message) if (!(invariant)) { Genode::error(message ": " #invariant); exit(1); }
#define NOT_IMPLEMENTED Genode::warning(__func__, " not implemented.");

namespace art {

    std::mutex* MemMap::mem_maps_lock_ = nullptr;

    using android::base::StringPrintf;

    template<typename A, typename B>
    static ptrdiff_t PointerDiff(A* a, B* b) {
      return static_cast<ptrdiff_t>(reinterpret_cast<intptr_t>(a) - reinterpret_cast<intptr_t>(b));
    }

    template<class Key, class T, AllocatorTag kTag, class Compare = std::less<Key>>
    using AllocationTrackingMultiMap =
        std::multimap<Key, T, Compare, TrackingAllocator<std::pair<const Key, T>, kTag>>;

    using Maps = AllocationTrackingMultiMap<void*, MemMap*, kAllocatorTagMaps>;
    static Maps* gMaps GUARDED_BY(MemMap::GetMemMapsLock()) = nullptr;

    bool GenodeFree(Genode::Ram_dataspace_capability *rdc,
                    std::string *error_msg) {
        if (rdc == nullptr) {
            if (error_msg) {
                *error_msg = "No dataspace allocated";
            }
            return false;
        };

        Genode::Env *env = &gart::genode_env();
        env->ram().free(*rdc);
    }

    bool GenodeUnmap(void* addr,
                     std::string *error_msg) {

        Genode::Env *env = &gart::genode_env();
        Genode::Region_map_client as(env->pd().address_space());

        try {
            as.detach(addr);
        } catch (...) {
            if (error_msg) {
                *error_msg = StringPrintf ("Error unmapping address %llx", addr);
            }
            return false;
        }
        return true;
    }

    void* GenodeMap(Genode::Ram_dataspace_capability *rdc,
                    void* addr,
                    size_t length,
                    bool read,
                    bool write,
                    bool exec,
                    std::string *error_msg) {

        Genode::Env *env = &gart::genode_env();

        if (rdc == nullptr) {
            if (error_msg) {
                *error_msg = "Invalid RAM dataspace";
            }
            return nullptr;
        }

        Genode::Dataspace_client ds(*rdc);
        Genode::Region_map_client as(env->pd().address_space());

        void *result = nullptr;
        try {
            result = as.attach(/* ds             */ *rdc,
                               /* size           */ 0,
                               /* offset         */ 0,
                               /* use_local_addr */ addr != nullptr,
                               /* local_addr     */ addr,
                               /* executable     */ exec,
                               /* writable       */ write);
        } catch (Genode::Region_map::Region_conflict) {
            if (error_msg) {
                *error_msg = "Region map conflict";
            }
        }
        return result;
    }

    Genode::Ram_dataspace_capability *GenodeAlloc(size_t length,
                                                   std::string *error_msg) {
        Genode::Env *env = &gart::genode_env();
        try {
            return new Genode::Ram_dataspace_capability(env->ram().alloc(length));
        } catch (...) {
            if (error_msg) {
                *error_msg = "Allocation error";
            }
            return nullptr;
        }
    }

#ifdef __LP64__
    void* GenodeMapLow(Genode::Ram_dataspace_capability *rdc,
                       size_t length,
                       bool read,
                       bool write,
                       bool exec,
                       std::string *error_msg) {

        size_t adjust = 0x1000;
        for (size_t b = 0;
             b < 0xfffff000;
             b += adjust)
        {
            void *result = GenodeMap (/* rdc       */ rdc,
                                      /* addr      */ reinterpret_cast<uint8_t *>(b),
                                      /* length    */ length,
                                      /* read      */ read,
                                      /* write     */ write,
                                      /* exec      */ exec,
                                      /* error_msg */ error_msg);
            if (result != nullptr) {
                return result;
            }
            adjust *= 2;
        }
        return nullptr;
    }
#endif // __LP64__

    void* GenodeMapInternal(Genode::Ram_dataspace_capability *rdc,
                            void* addr,
                            size_t length,
                            bool read,
                            bool write,
                            bool exec,
                            bool low_4gb,
                            std::string *error_msg) {

#ifdef __LP64__
        if (low_4gb && addr == nullptr) {
            return GenodeMapLow(/* rdc       */ rdc,
                                /* length    */ length,
                                /* read      */ read,
                                /* write     */ write,
                                /* exec      */ exec,
                                /* error_msg */ error_msg);
        }

        if (reinterpret_cast<uint64_t>(addr) > 0xfffff000ULL) {
            if (error_msg) {
                *error_msg = "Non-lowmem fixed addres and low_4gb set";
            }
            return nullptr;
        }
#endif
        return GenodeMap(/* rdc       */ rdc,
                         /* addr      */ addr,
                         /* length    */ length,
                         /* read      */ read,
                         /* write     */ write,
                         /* exec      */ exec,
                         /* error_msg */ error_msg);
    }

    bool MemMap::ContainedWithinExistingMap(uint8_t* ptr, size_t size, std::string* error_msg) {
      uintptr_t begin = reinterpret_cast<uintptr_t>(ptr);
      uintptr_t end = begin + size;

      {
        std::lock_guard<std::mutex> mu(*mem_maps_lock_);
        for (auto& pair : *gMaps) {
          MemMap* const map = pair.second;
          if (begin >= reinterpret_cast<uintptr_t>(map->Begin()) &&
              end <= reinterpret_cast<uintptr_t>(map->End())) {
            return true;
          }
        }
      }

      if (error_msg != nullptr) {
        *error_msg = StringPrintf("Requested region 0x%08" PRIx16 "-0x%08" PRIx16 " does not overlap "
                                  "any existing map.", begin, end);
      }
      return false;
    }


    MemMap::MemMap(const std::string& name,
                   uint8_t* begin,
                   size_t size,
                   void* base_begin,
                   size_t base_size,
                   int prot,
                   bool reuse,
                   Genode::Ram_dataspace_capability *rdc,
                   size_t redzone_size) : name_(name)
                                        , begin_(begin)
                                        , size_(size)
                                        , base_begin_(base_begin)
                                        , base_size_(base_size)
                                        , prot_(prot)
                                        , reuse_(reuse)
                                        , already_unmapped_(false)
                                        , redzone_size_(redzone_size)
                                        , rdc_(rdc)
    {
        if (size_ == 0) {
            CHECK(begin_ == nullptr);
            CHECK(base_begin_ == nullptr);
            CHECK_EQ(base_size_, 0U);
            return;
        }

        CHECK(begin_ != nullptr);
        CHECK(base_begin_ != nullptr);
        CHECK_NE(base_size_, 0U);

        // Add it to gMaps.
        std::lock_guard<std::mutex> mu(*mem_maps_lock_);
        DCHECK(gMaps != nullptr);
        gMaps->insert(std::make_pair(base_begin_, this));
    };

    MemMap::~MemMap()
    {
        if (base_begin_ == nullptr && base_size_ == 0) {
            return;
        }

        std::string error_msg;
        if (!reuse_) {
            if (!already_unmapped_) {
                if (!GenodeUnmap(base_begin_, &error_msg)) {
                    PLOG(FATAL) << "Unmapping failed: " << error_msg;
                }
                Genode::Env *env = &gart::genode_env();
                env->ram().free(*rdc_);
            }
        }

        // Remove it from gMaps.
        std::lock_guard<std::mutex> mu(*mem_maps_lock_);
        bool found = false;
        DCHECK(gMaps != nullptr);
        for (auto it = gMaps->lower_bound(base_begin_), end = gMaps->end();
             it != end && it->first == base_begin_;
             ++it)
        {
            if (it->second == this) {
                found = true;
                gMaps->erase(it);
                break;
            }
        }
        CHECK(found) << ": MemMap not found";
    }

    MemMap* MemMap::MapAnonymous(const char* name,
                                  uint8_t* expected_ptr,
                                 size_t byte_count,
                                 int prot,
                                 bool low_4gb,
                                 bool reuse,
                                 std::string* error_msg,
                                 bool use_ashmem)
    {
        Genode::Ram_dataspace_capability *rdc;
        size_t page_aligned_byte_count = RoundUp(byte_count, kPageSize);

        if (reuse) {
            CHECK(expected_ptr != nullptr);
            DCHECK(ContainedWithinExistingMap(expected_ptr, byte_count, error_msg)) << *error_msg;
        }

        rdc = GenodeAlloc(byte_count, error_msg);
        if (rdc == nullptr) {
            return nullptr;
        }

        bool read  = prot & PROT_READ;
        bool write = prot & PROT_WRITE;
        bool exec  = prot & PROT_EXEC;

          void* actual = GenodeMapInternal(/* rdc       */ rdc,
                                           /* addr      */ reinterpret_cast<void *>(expected_ptr),
                                           /* length    */ page_aligned_byte_count,
                                           /* read      */ read,
                                           /* write     */ write,
                                           /* exec      */ exec,
                                           /* low_4gb   */ low_4gb,
                                           /* error_msg */ error_msg);
        if (actual == nullptr) {
            return nullptr;
        }

        if (!read) {
            if (write || exec) {
                if (error_msg) {
                    *error_msg = "Write/exec-only regions unsupported";
                    Genode::warning(error_msg->c_str());
                }
                return nullptr;
            }
            // Unmap page
            if (!GenodeUnmap(actual, error_msg)) {
                return nullptr;
            }
        }

        return new MemMap(name,
                          reinterpret_cast<uint8_t*>(actual),
                          byte_count,
                          actual,
                          page_aligned_byte_count,
                          prot,
                          reuse,
                          rdc);
    }

    MemMap* MemMap::RemapAtEnd(uint8_t* new_end,
                               const char* tail_name,
                               int tail_prot,
                               std::string* error_msg,
                               bool use_ashmem)
    {
        NOT_IMPLEMENTED;
        return nullptr;
    }

    void MemMap::TryReadable()
    {
        NOT_IMPLEMENTED;
    }

    void MemMap::Init()
    {
          if (mem_maps_lock_ != nullptr) {
            return;
          }
          mem_maps_lock_ = new std::mutex();
          DCHECK(gMaps == nullptr);
          gMaps = new Maps;
    }

    void MemMap::Shutdown()
    {
          if (mem_maps_lock_ == nullptr) {
            return;
          }
          {
            DCHECK(gMaps != nullptr);
            delete gMaps;
            gMaps = nullptr;
          }
          delete mem_maps_lock_;
          mem_maps_lock_ = nullptr;
    }

    void MemMap::DumpMaps(std::ostream& os, bool terse)
    {
        NOT_IMPLEMENTED;
    }

    MemMap* MemMap::MapDummy(const char* name, uint8_t* addr, size_t byte_count)
    {
        NOT_IMPLEMENTED;
        return nullptr;
    }

    bool MemMap::Protect(int prot)
    {
        if (base_begin_ == nullptr && base_size_ == 0) {
            prot_ = prot;
            return true;
        }

        std::string tmp_error;
        DCHECK(GenodeUnmap(base_begin_, &tmp_error)) << tmp_error;
        void *new_address = GenodeMap(/* rdc       */ rdc_,
                                      /* addr      */ base_begin_,
                                      /* length    */ base_size_,
                                      /* read      */ prot & PROT_READ,
                                      /* write     */ prot & PROT_WRITE,
                                      /* exec      */ prot & PROT_EXEC,
                                      /* error_msg */ &tmp_error);
        DCHECK(new_address != nullptr) << tmp_error;
        DCHECK_EQ(base_begin_, new_address) << "remapping at wrong address";
        prot_ = prot;
        return true;
    }

    void MemMap::SetSize(size_t new_size)
    {
        CHECK_LE(new_size, size_);
        size_t new_base_size = RoundUp(new_size + static_cast<size_t>(PointerDiff(Begin(), BaseBegin())),
                                       kPageSize);
        if (new_base_size == base_size_) {
            size_ = new_size;
            return;
        }
        CHECK_LT(new_base_size, base_size_);

        std::string tmp_error;
        CHECK_NE(GenodeUnmap(base_begin_, &tmp_error), 0) << tmp_error;
        void *new_address = GenodeMap(/* rdc       */ rdc_,
                                      /* addr      */ base_begin_,
                                      /* length    */ new_base_size,
                                      /* read      */ prot_ & PROT_READ,
                                      /* write     */ prot_ & PROT_WRITE,
                                      /* exec      */ prot_ & PROT_EXEC,
                                      /* error_msg */ &tmp_error);
        DCHECK(new_address != nullptr) << tmp_error;
        DCHECK_EQ(base_begin_, new_address) << "remapping at wrong address";

        base_size_ = new_base_size;
        size_ = new_size;
    }

    MemMap* MemMap::MapFileAtAddress(uint8_t* addr,
                                     size_t byte_count,
                                     int prot,
                                     int flags,
                                     int fd,
                                     off_t start,
                                     bool low_4gb,
                                     bool reuse,
                                     const char* filename,
                                     std::string* error_msg)
    {
        NOT_IMPLEMENTED;
        return nullptr;
    }

    void MemMap::MadviseDontNeedAndZero()
    {
        if (base_begin_ == nullptr && base_size_ == 0) {
            return;
        }

        if (!kMadviseZeroes) {
            memset(base_begin_, 0, base_size_);
        }
    }

    void MemMap::AlignBy(size_t size)
    {
        NOT_IMPLEMENTED;
    }

    bool MemMap::CheckNoGaps(MemMap* begin_map, MemMap* end_map)
    {
        NOT_IMPLEMENTED;
    }

    void ZeroAndReleasePages(void* address, size_t length)
    {
        NOT_IMPLEMENTED;
    }

    void* MemMap::MapInternal(void* addr,
                              size_t length,
                              int prot,
                              int flags,
                              int fd,
                              off_t offset,
                              bool low_4gb) {
        NOT_IMPLEMENTED;
        return nullptr;
    }
}
