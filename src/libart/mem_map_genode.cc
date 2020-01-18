// Genode includes
#include <base/log.h>
#include <base/exception.h>
#include <util/string.h>
#include <region_map/region_map.h>

// Android includes
#include <android-base/stringprintf.h>
#include "base/allocator.h"
#include "base/logging.h"
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

#define NOT_IMPLEMENTED Genode::warning(__func__, " not implemented.");

namespace art {

    using android::base::StringPrintf;

    class DataSpace {

        private:
            Genode::Env *env_;
            Genode::Ram_dataspace_capability *rdc_;
            void *addr_;

            bool MapInternal(void* addr,
                             size_t length,
                             size_t offset,
                             bool read,
                             bool write,
                             bool exec,
                             std::string *error_msg) {
                DCHECK(env_ != nullptr);
                DCHECK(rdc_ != nullptr);

                Genode::Dataspace_client ds(*rdc_);
                Genode::Region_map_client as(env_->pd().address_space());

                try {
                    addr_ = as.attach(/* ds             */ *rdc_,
                                      /* size           */ length,
                                      /* offset         */ offset,
                                      /* use_local_addr */ addr != nullptr,
                                      /* local_addr     */ addr,
                                      /* executable     */ exec,
                                      /* writable       */ write);
                    return true;
                } catch (Genode::Region_map::Region_conflict) {
                    if (error_msg != nullptr) {
                        *error_msg = "Region map conflict";
                    }
                } catch (Genode::Region_map::Invalid_dataspace) {
                    if (error_msg != nullptr) {
                        *error_msg = "Invalid dataspace";
                    }
                } catch (Genode::Out_of_ram) {
                    if (error_msg != nullptr) {
                        *error_msg = "Out of RAM";
                    }
                } catch (Genode::Out_of_caps) {
                    if (error_msg != nullptr) {
                        *error_msg = "Out of capabilities";
                    }
                } catch (...) {
                    if (error_msg != nullptr) {
                        *error_msg = "Unexpected exception";
                    }
                }
                return false;
            }

#ifdef __LP64__
            bool MapLow(size_t length,
                        size_t offset,
                        bool read,
                        bool write,
                        bool exec,
                        std::string *error_msg) {

                DCHECK(length > 0) << "MapLow called with zero length";

                size_t adjust = 0x1000;
                for (size_t b = 0;
                     b < 0xfffff000;
                     b += adjust)
                {
                    bool success = MapInternal (/* addr      */ reinterpret_cast<uint8_t *>(b),
                                                /* length    */ length,
                                                /* offset    */ offset,
                                                /* read      */ read,
                                                /* write     */ write,
                                                /* exec      */ exec,
                                                /* error_msg */ error_msg);
                    if (success) {
                        return true;
                    }
                    adjust *= 2;
                }
                return false;
            }
#endif // __LP64__

        public:

            DataSpace() : env_(&gart::genode_env())
                        , rdc_(nullptr)
                        { }

            ~DataSpace() {
                std::string error_msg;
                DCHECK(Unmap(&error_msg)) << error_msg;
                Free();
            }

            void *Addr() { return addr_; }
            uint8_t *UAddr() { return reinterpret_cast<uint8_t *>(addr_); }

            void Free() {
                DCHECK(env_ != nullptr);
                DCHECK(rdc_ != nullptr);
                env_->ram().free(*rdc_);
                rdc_ = nullptr;
            }

            bool Unmap(std::string *error_msg) {
                DCHECK(env_ != nullptr);

                if (addr_ == nullptr) {
                    return true;
                }

                Genode::Region_map_client as(env_->pd().address_space());
                try {
                    as.detach(addr_);
                    addr_ = nullptr;
                } catch (...) {
                    if (error_msg != nullptr) {
                        *error_msg = StringPrintf ("Error unmapping address %llx", addr_);
                    }
                    return false;
                }
                return true;
            }

            bool Alloc(size_t length,
                       std::string *error_msg) {
                DCHECK(env_ != nullptr);

                try {
                    rdc_ = new Genode::Ram_dataspace_capability(env_->ram().alloc(length));
                } catch (...) {
                    if (error_msg != nullptr) {
                        *error_msg = "Allocation error";
                    }
                    return false;
                }
                return true;
            }

            bool Map(void* addr,
                     size_t length,
                     size_t offset,
                     bool read,
                     bool write,
                     bool exec,
                     bool low_4gb,
                     std::string *error_msg) {

                DCHECK(length > 0) << "Map called with zero length";

#ifdef __LP64__
                if (low_4gb && addr == nullptr) {
                    return MapLow(/* length    */ length,
                                  /* offset    */ offset,
                                  /* read      */ read,
                                  /* write     */ write,
                                  /* exec      */ exec,
                                  /* error_msg */ error_msg);
                }

                if (reinterpret_cast<uint64_t>(addr) > 0xfffff000ULL) {
                    if (error_msg != nullptr) {
                        *error_msg = "Non-lowmem fixed addres and low_4gb set";
                    }
                    return false;
                }
#endif
                return MapInternal(/* addr      */ addr,
                                   /* length    */ length,
                                   /* offset    */ offset,
                                   /* read      */ read,
                                   /* write     */ write,
                                   /* exec      */ exec,
                                   /* error_msg */ error_msg);
            }

            bool Remap(size_t length,
                       size_t offset,
                       bool read,
                       bool write,
                       bool exec,
                       std::string *error_msg) {

                void *old_addr = addr_;
                // FIXME: Take lock between Unmap/Map!
                if (!Unmap(error_msg)) {
                    return false;
                }
                bool success = Map(/* addr      */ old_addr,
                                   /* length    */ length,
                                   /* offset    */ offset,
                                   /* read      */ read,
                                   /* write     */ write,
                                   /* exec      */ exec,
                                   /* low_4gb   */ false,
                                   /* error_msg */ error_msg);
                DCHECK(success) << error_msg;
                DCHECK_EQ(Addr(), old_addr) << "remapping at wrong address";
                return true;
            }
    };

    std::mutex* MemMap::mem_maps_lock_ = nullptr;

    template<typename A, typename B>
    static ptrdiff_t PointerDiff(A* a, B* b) {
      return static_cast<ptrdiff_t>(reinterpret_cast<intptr_t>(a) - reinterpret_cast<intptr_t>(b));
    }

    template<class Key, class T, AllocatorTag kTag, class Compare = std::less<Key>>
    using AllocationTrackingMultiMap =
        std::multimap<Key, T, Compare, TrackingAllocator<std::pair<const Key, T>, kTag>>;

    using Maps = AllocationTrackingMultiMap<void*, MemMap*, kAllocatorTagMaps>;
    static Maps* gMaps GUARDED_BY(MemMap::GetMemMapsLock()) = nullptr;

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
                   DataSpace *memory,
                   size_t redzone_size) : name_(name)
                                        , begin_(begin)
                                        , size_(size)
                                        , base_begin_(base_begin)
                                        , base_size_(base_size)
                                        , prot_(prot)
                                        , reuse_(reuse)
                                        , already_unmapped_(false)
                                        , redzone_size_(redzone_size)
                                        , memory_(memory)
    {
        if (size_ == 0) {
            CHECK(begin_ == nullptr);
            CHECK(base_begin_ == nullptr);
            CHECK_EQ(base_size_, 0U);
            return;
        }

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
            if (memory_ != nullptr) {
                delete memory_;
                memory_ = nullptr;
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
        VLOG(heap) << __FUNCTION__ << " @" << (void *)expected_ptr << " size=" << byte_count << " prot=" << prot << " [" << name << "]";
        size_t page_aligned_byte_count = RoundUp(byte_count, kPageSize);

        if (byte_count == 0) {
            return new MemMap(/* name       */ name,
                              /* begin      */ nullptr,
                              /* size       */ 0,
                              /* base_begin */ nullptr,
                              /* base_size  */ 0,
                              /* prot       */ prot,
                              /* reuse      */ false,
                              /* memory     */ nullptr);
        }

        if (reuse) {
            CHECK(expected_ptr != nullptr);
            DCHECK(ContainedWithinExistingMap(expected_ptr, byte_count, error_msg)) << *error_msg;
        }

        DataSpace *memory = new DataSpace();
        DCHECK(memory != nullptr);
        if (!memory->Alloc(byte_count, error_msg)) {
            return nullptr;
        }

        bool read  = prot & PROT_READ;
        bool write = prot & PROT_WRITE;
        bool exec  = prot & PROT_EXEC;

        bool success = memory->Map(/* addr      */ reinterpret_cast<void *>(expected_ptr),
                                   /* length    */ page_aligned_byte_count,
                                   /* offset    */ 0,
                                   /* read      */ read,
                                   /* write     */ write,
                                   /* exec      */ exec,
                                   /* low_4gb   */ low_4gb,
                                   /* error_msg */ error_msg);
        if (!success) {
            return nullptr;
        }

        if (expected_ptr != nullptr) {
            DCHECK(expected_ptr == memory->Addr());
        }

        auto result = new MemMap(/* name       */ name,
                                 /* begin      */ memory->UAddr(),
                                 /* size       */ byte_count,
                                 /* base_begin */ memory->Addr(),
                                 /* base_size  */ page_aligned_byte_count,
                                 /* prot       */ prot,
                                 /* reuse      */ reuse,
                                 /* memory     */ memory);

        if (!read) {
            if (write || exec) {
                if (error_msg != nullptr) {
                    *error_msg = "Write/exec-only regions unsupported";
                    Genode::warning(error_msg->c_str());
                }
                return nullptr;
            }
            // Unmap region
            VLOG(heap) << __FUNCTION__ << ": unmapping non-readable page";
            if (!memory->Unmap(error_msg)) {
                return nullptr;
            }
        }

        return result;
    }

    MemMap* MemMap::RemapAtEnd(uint8_t* new_end,
                               const char* tail_name,
                               int tail_prot,
                               std::string* error_msg,
                               bool use_ashmem)
    {
        DCHECK_GE(new_end, Begin());
        DCHECK_LE(new_end, End());
        DCHECK_LE(begin_ + size_, reinterpret_cast<uint8_t*>(base_begin_) + base_size_);
        DCHECK_ALIGNED(begin_, kPageSize);
        DCHECK_ALIGNED(base_begin_, kPageSize);
        DCHECK_ALIGNED(reinterpret_cast<uint8_t*>(base_begin_) + base_size_, kPageSize);
        DCHECK_ALIGNED(new_end, kPageSize);

        uint8_t* old_end = begin_ + size_;
        uint8_t* old_base_end = reinterpret_cast<uint8_t*>(base_begin_) + base_size_;
        uint8_t* new_base_end = new_end;

        if (new_base_end == old_base_end) {
            return new MemMap(/* name       */ tail_name,
                              /* begin      */ nullptr,
                              /* size       */ 0,
                              /* base_begin */ nullptr,
                              /* base_size  */ 0,
                              /* prot       */ tail_prot,
                              /* reuse      */ false,
                              /* rdc        */ nullptr);
        }

        size_ = new_end - reinterpret_cast<uint8_t*>(begin_);
        base_size_ = new_base_end - reinterpret_cast<uint8_t*>(base_begin_);
        DCHECK_LE(begin_ + size_, reinterpret_cast<uint8_t*>(base_begin_) + base_size_);
        size_t tail_size = old_end - new_end;
        uint8_t* tail_base_begin = new_base_end;
        size_t tail_base_size = old_base_end - new_base_end;
        DCHECK_EQ(tail_base_begin + tail_base_size, old_base_end);
        DCHECK_ALIGNED(tail_base_size, kPageSize);

        if (!memory_->Unmap(error_msg)) {
            return nullptr;
        }

        bool success = memory_->Map(/* addr      */ base_begin_,
                                    /* length    */ base_size_,
                                    /* offset    */ 0,
                                    /* read      */ prot_ & PROT_READ,
                                    /* write     */ prot_ & PROT_WRITE,
                                    /* exec      */ prot_ & PROT_EXEC,
                                    /* low_4gb   */ false,
                                    /* error_msg */ error_msg);
        DCHECK(success) << (error_msg == nullptr) ? "" : *error_msg;
        DCHECK_EQ(base_begin_, memory_->Addr());

        success = memory_->Map(/* addr      */ tail_base_begin,
                               /* length    */ tail_base_size,
                               /* offset    */ base_size_,
                               /* read      */ tail_prot & PROT_READ,
                               /* write     */ tail_prot & PROT_WRITE,
                               /* exec      */ tail_prot & PROT_EXEC,
                               /* low_4gb   */ false,
                               /* error_msg */ error_msg);
        DCHECK(success) << (error_msg == nullptr) ? "" : *error_msg;

        return new MemMap(/* name       */ tail_name,
                          /* begin      */ memory_->UAddr(),
                          /* size       */ tail_base_size,
                          /* base_begin */ memory_->Addr(),
                          /* base_size  */ tail_base_size,
                          /* prot       */ tail_prot,
                          /* reuse      */ false,
                          /* rdc        */ memory_);
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
        bool success = memory_->Remap (/* length    */ base_size_,
                                       /* offset    */ 0, // FIXME: maintain offset!
                                       /* read      */ prot_ & PROT_READ,
                                       /* write     */ prot_ & PROT_WRITE,
                                       /* exec      */ prot_ & PROT_EXEC,
                                       /* error_msg */ &tmp_error);
        DCHECK(success) << tmp_error;
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
        bool success = memory_->Remap (/* length    */ new_base_size,
                                       /* offset    */ 0, // FIXME: maintain offset!
                                       /* read      */ prot_ & PROT_READ,
                                       /* write     */ prot_ & PROT_WRITE,
                                       /* exec      */ prot_ & PROT_EXEC,
                                       /* error_msg */ &tmp_error);
        DCHECK(success) << tmp_error;

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
        CHECK_NE(0, prot);
        if (byte_count == 0) {
            return new MemMap(/* name       */ filename,
                              /* begin      */ nullptr,
                              /* size       */ 0,
                              /* base_begin */ nullptr,
                              /* base_size  */ 0,
                              /* prot       */ prot,
                              /* reuse      */ false,
                              /* rdc        */ nullptr);
        }

        int page_offset = start % kPageSize;
        off_t page_aligned_offset = start - page_offset;
        size_t page_aligned_byte_count = RoundUp(byte_count + page_offset, kPageSize);
        uint8_t* page_aligned_expected = (addr == nullptr) ? nullptr : (addr - page_offset);

        MemMap* result = MapAnonymous(/* name         */ filename,
                                      /* expected_ptr */ page_aligned_expected,
                                      /* byte_count   */ page_aligned_byte_count,
                                      /* prot         */ prot | PROT_WRITE,
                                      /* low_4gb      */ low_4gb,
                                      /* reuse        */ reuse,
                                      /* error_msg    */ error_msg,
                                      /* use_ashmem   */ false);
        if (result == nullptr) {
            return nullptr;
        }

        DCHECK(result->Size() >= byte_count);

        off_t old_offset = lseek(fd, 0, SEEK_SET);
        size_t offset = 0;

        while (offset < byte_count)
        {
            ssize_t bytes = read(fd, result->Begin() + offset, byte_count - offset);
            if (bytes < 0) {
                delete result;
                *error_msg = strerror(errno);
                return nullptr;
            }

            if (bytes == 0) {
                break;
            }
            DCHECK(offset <= byte_count - bytes);
            offset += bytes;
        }

        lseek(fd, old_offset, SEEK_SET);
        result->Protect(prot);
        return result;
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
