// Genode includes
#include <base/log.h>
#include <base/exception.h>
#include <util/string.h>
#include <region_map/region_map.h>

// Android includes
#include <android-base/stringprintf.h>
#include <base/bit_utils.h>
#include <base/globals.h>

// stdc++ includes
#include <atomic>
#include <list>
#include <iostream>

// GART includes
#include <gart/env.h>

// local includes
#include "mem_map.h"

class Assertion_failed : Genode::Exception { };

#define assert(invariant, message) if (!(invariant)) { Genode::error(message ": " #invariant); exit(1); }

static std::atomic_size_t lowmem_base = 0x1000;

namespace art {

    std::mutex* MemMap::mem_maps_lock_ = nullptr;
    static std::list<MemMap> mem_map_;

    class Error
    {
        private:
            const std::string &message_;
        public:
            Error(const std::string &message) : message_(message) { }
            void message(std::string *m) { if (m != nullptr) { *m = message_; } }
    };

    enum { PROT_READ = 1, PROT_WRITE = 2, PROT_EXEC = 4, PROT_LOWMEM = 8 };

    MemMap::MemMap(const std::string& name,
                   uint8_t* begin,
                   size_t size,
                   void* base_begin,
                   size_t base_size,
                   int prot,
                   bool reuse,
                   size_t redzone_size) : name_(name)
                                        , prot_(prot & (~PROT_LOWMEM))
                                        , reuse_(reuse)
                                        , already_unmapped_(false)
                                        , redzone_size_(redzone_size)
                                        , env_(gart::genode_env())
                                        , ram_ds_cap_(env_.ram().alloc(size))
                                        , ds_(ram_ds_cap_)
                                        , address_space_(env_.pd().address_space())
    {
        //Genode::log(__func__, ": ", name.c_str(), " @ ", Genode::Hex(reinterpret_cast<unsigned long>(begin)), " size=", size, " write=", bool(prot & PROT_WRITE), " exec=", bool(prot & PROT_EXEC));
        assert ((prot_ & ~0xfUL) == 0, "Unsupported protection bits");
        assert (reuse == 0, "Reuse not implemented");
        assert ((reinterpret_cast<unsigned long>(begin) & 0xfff) == 0, "Address not page-aligned")

        bool lowmem =
#ifdef __LP64__
            (prot & PROT_LOWMEM) > 0;
#else
            false;
#endif

        if (!(prot_ & PROT_READ))
        {
            // Genode::warning("Non-accessible mapping unsupported, adding read permission");
            prot_ |= PROT_READ;
        }

        size_ = size;
        base_size_ = ds_.size();

        int tries = 0;
        bool done = false;
        if (lowmem && begin == nullptr)
        {
            size_t adjust = 0x1000;
            for (size_t b = lowmem_base; !done && b < (1UL << 32 - ds_.size()); b += adjust, tries += 1)
            {
                try {
                    begin_ = address_space_.attach(ram_ds_cap_, 0, 0, true, reinterpret_cast<void *>(b), prot_ & PROT_EXEC, prot_ & PROT_WRITE);
                    done = true;
                } catch (Genode::Region_map::Region_conflict) { adjust *= 2; }
            }
            if (!done)
            {
                Genode::error("Error mapping low-mem");
                throw Error("Error mapping low-mem");
            }
            size_t new_base = lowmem_base.load() + ds_.size();
            lowmem_base.store(new_base);
        } else
        {
            begin_= address_space_.attach(ram_ds_cap_, 0, 0, begin != nullptr, begin, prot_ & PROT_EXEC, prot_ & PROT_WRITE);
        }

        base_begin_ = (void *)begin_;

        // Initialize memory
        void *b = address_space_.attach(ram_ds_cap_, 0, 0, false, nullptr, false, true);
        Genode::memset(b, 0, ds_.size());
        address_space_.detach(b);
    };

    MemMap::~MemMap()
    {
    }

    MemMap * MemMap::Detach() {
        address_space_.detach(base_begin_);
        env_.ram().free(ram_ds_cap_);
    }

    void Dump(std::string message) {
        bool error = false;
        uint8_t *last = 0;
        fprintf(stderr, "   >>> MAP: %s\n", message.c_str());
        for (auto it = mem_map_.begin(); it != mem_map_.end(); it++) {
            if (it == mem_map_.end()) {
                break;
            }
            if ((*it).BaseBegin() < last) {
                fprintf(stderr, "      ERROR: Next entry not continuous!\n");
                error = true;
            }
            int prot = (*it).GetProtect();
            fprintf(stderr, "      %8.8llx %8.0llx %c%c - %s\n", (*it).BaseBegin(), (*it).BaseSize(), prot & PROT_WRITE ? 'w' : '_', prot & PROT_EXEC ? 'x' : '_', (*it).GetName().c_str());
            last = reinterpret_cast<uint8_t *>((*it).BaseBegin()) + (*it).BaseSize();
        }
        fprintf(stderr, "   <<< MAP: %s\n", message.c_str());
        if (error) {
            abort();
        }
    }

    bool Insert(MemMap* entry,
                const char *name,
                std::string* error_msg)
    {
        auto elem = mem_map_.begin();
        while (elem != mem_map_.end()) {
            if ((*elem).BaseBegin() > entry->BaseBegin()) {
                auto next = std::next(elem, 1);
                if (next != mem_map_.end() && entry->BaseEnd() >= (*next).BaseBegin()) {
                    if (error_msg) {
                        *error_msg = "Region '" + std::string(name) + "' overlaps '" + (*next).GetName() + "'";
                    }
                    return false;
                }
                break;
            }
            if ((*elem).BaseBegin() == entry->BaseBegin()
                && (*elem).BaseSize() == entry->BaseSize()
                && (*elem).GetProtect() == entry->GetProtect()) {
                // Already inserted
                return true;
            }

            if ((*elem).BaseBegin() == entry->BaseBegin()) {
                if (error_msg) {
                    *error_msg = "Conflicting region '" + std::string(name) + "' (" + (*elem).GetName() + ")";
                }
                return false;
            }
            elem++;
        }
        mem_map_.insert(elem, *entry);
        return true;
    };

    MemMap * MemMap::Split(uint8_t *addr,
                           size_t size,
                           int prot,
                           std::string name,
                           std::string* error_msg)
    {
        auto it = mem_map_.begin();
        while (it != mem_map_.end()) {
            if (addr >= (*it).BaseBegin() && addr <= (*it).BaseEnd()) {
                break;
            }
            it++;
        }
        if (it == mem_map_.end()) {
            *error_msg = "Error splitting region '" + name + "': region not found";
            return nullptr;
        }

        // detach
        std::string  old_name(GetName());
        uint8_t     *old_base_begin = reinterpret_cast<uint8_t *>(BaseBegin());
        int          old_size       = BaseSize();
        int          old_prot       = GetProtect();
        Detach();

        // left region
        if (old_base_begin < addr) {
            auto left = new MemMap (/* name         */ old_name,
                                    /* begin        */ old_base_begin,
                                    /* size         */ addr - old_base_begin,
                                    /* base_begin   */ old_base_begin,
                                    /* base_size    */ addr - old_base_begin,
                                    /* prot         */ old_prot,
                                    /* reuse        */ false,
                                    /* redzone_size */ 0);
            mem_map_.insert(it, *left);
        }

        // middle region
        auto middle = new MemMap (/* name         */ name,
                                     /* begin        */ addr,
                                     /* size         */ size,
                                     /* base_begin   */ addr,
                                     /* base_size    */ size,
                                     /* prot         */ prot,
                                     /* reuse        */ false,
                                     /* redzone_size */ 0);
        mem_map_.insert(it, *middle);

        // right region
        if (old_base_begin + old_size >= middle->BaseEnd()) {
            size_t s = old_base_begin + old_size - reinterpret_cast<uint8_t *>(middle->BaseEnd());
            auto right = new MemMap (/* name         */ old_name,
                                     /* begin        */ old_base_begin + old_size,
                                     /* size         */ s,
                                     /* base_begin   */ old_base_begin + old_size,
                                     /* base_size    */ s,
                                     /* prot         */ old_prot,
                                     /* reuse        */ false,
                                     /* redzone_size */ 0);
            mem_map_.insert(it, *right);
        }
        return middle;
    }

    MemMap* MemMap::MapAnonymous(const char* name,
                                 uint8_t* addr,
                                 size_t byte_count,
                                 int prot,
                                 bool low_4gb,
                                 bool reuse,
                                 std::string* error_msg,
                                 bool use_ashmem)
    {
        try
        {
            std::lock_guard<std::mutex> mu(*mem_maps_lock_);
            if (addr != nullptr && reuse) {
                for (auto it = mem_map_.begin(); it != mem_map_.end(); it++) {
                    if (it == mem_map_.end() || (*it).BaseBegin() > addr) {
                        break;
                    }
                    if (addr >= (*it).BaseBegin() && addr <= (*it).BaseEnd()) {
                        return (*it).Split(addr, byte_count, prot, name, error_msg);
                    }
                }
                if (error_msg) {
                    *error_msg = "Region '" + std::string(name) + "' not found";
                }
                return nullptr;
            }
            auto result = new MemMap (/* name         */ name,
                                      /* begin        */ addr,
                                      /* size         */ byte_count,
                                      /* base_begin   */ addr,
                                      /* base_size    */ byte_count,
                                      /* prot         */ prot | (low_4gb ? PROT_LOWMEM : 0),
                                      /* reuse        */ false,
                                      /* redzone_size */ 0);
            if (!Insert(result, name, error_msg)) {
                return nullptr;
            };
            return result;
        } catch (Error e)
        {
            e.message(error_msg);
            return nullptr;
        } catch (...)
        {
            *error_msg = "Unknown exception when mapping memory";
            return nullptr;
        }
    }

    MemMap* MemMap::RemapAtEnd(uint8_t* new_end,
                               const char* tail_name,
                               int tail_prot,
                               std::string* error_msg,
                               bool use_ashmem)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        return nullptr;
    }

    void MemMap::TryReadable()
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
    }

    void MemMap::Init()
    {
        if (mem_maps_lock_ != nullptr)
        {
            return;
        }
        mem_maps_lock_ = new std::mutex();
    }

    void MemMap::Shutdown()
    {
        if (mem_maps_lock_ == nullptr)
        {
            return;
        }
        delete mem_maps_lock_;
        mem_maps_lock_ = nullptr;
        mem_map_.clear();
    }

    void MemMap::DumpMaps(std::ostream& os, bool terse)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
    }

    MemMap* MemMap::MapDummy(const char* name, uint8_t* addr, size_t byte_count)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        return nullptr;
    }

    bool MemMap::Protect(int prot)
    {
        std::lock_guard<std::mutex> mu(*mem_maps_lock_);
        if (prot == prot_)
        {
            return true;
        }

        try
        {
            address_space_.detach(base_begin_);
            base_begin_= address_space_.attach(ram_ds_cap_, 0, 0, true, base_begin_, prot_ & PROT_EXEC, prot_ & PROT_WRITE);
        } catch (Error e)
        {
            Genode::error("Error changing protection from ", prot_, " to ", prot);
            return false;
        }
        prot_ = prot;
        return true;
    }

#define PointerDiff(a, b) (size_t)(reinterpret_cast<uintptr_t>(a) - reinterpret_cast<uintptr_t>(b))

    void MemMap::SetSize(size_t new_size)
    {
        std::lock_guard<std::mutex> mu(*mem_maps_lock_);
        size_t new_base_size = RoundUp(new_size + PointerDiff(begin_, base_begin_), kPageSize);
        if (new_base_size == ds_.size())
        {
            size_ = new_size;
            return;
        }

        if (new_base_size > ds_.size())
        {
            Genode::error("Error increasing memmap size from ", ds_.size(), " to ", new_base_size);
            return;
        }

        address_space_.detach(Genode::Region_map::Local_addr(base_begin_));
        Genode::Region_map::Local_addr result = address_space_.attach(/* ds */ ram_ds_cap_,
                                                                      /* size */ new_base_size,
                                                                      /* offset */ 0,
                                                                      /* use_local_addr */ true,
                                                                      /* local_addr */ begin_,
                                                                      /* executable */ prot_ & PROT_EXEC,
                                                                      /* writeable */ prot_ & PROT_WRITE);
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
        MemMap *result;
        result = MapAnonymous (/* name       */ filename,
                               /* addr       */ addr,
                               /* size       */ byte_count,
                               /* prot       */ prot | PROT_WRITE | (low_4gb ? PROT_LOWMEM : 0),
                               /* low_4gb    */ low_4gb,
                               /* reuse      */ reuse,
                               /* error_msg  */ error_msg,
                               /* use_ashmem */ false);
        if (result == nullptr)
        {
            return nullptr;
        }

        DCHECK(result->Size() >= byte_count);

        off_t old_offset = lseek(fd, 0, SEEK_SET);
        size_t offset = 0;

        while (offset < byte_count)
        {
            ssize_t bytes = read(fd, result->Begin() + offset, byte_count - offset);
            if (bytes < 0)
            {
                delete result;
                *error_msg = strerror(errno);
                return nullptr;
            }

            if (bytes == 0)
            {
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
        std::lock_guard<std::mutex> mu(*mem_maps_lock_);
        if (base_begin_ == nullptr && ds_.size() == 0)
        {
            return;
        }

        if (prot_ & PROT_WRITE && !kMadviseZeroes)
        {
            Genode::memset(base_begin_, 0, ds_.size());
        }
    }

    void MemMap::AlignBy(size_t size)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
    }

    bool MemMap::CheckNoGaps(MemMap* begin_map, MemMap* end_map)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        return false;
    }

    void ZeroAndReleasePages(void* address, size_t length)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
    }

}
