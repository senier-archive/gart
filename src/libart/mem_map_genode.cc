// Genode includes
#include <base/log.h>
#include <base/exception.h>
#include <dataspace/client.h>
#include <util/string.h>
#include <region_map/region_map.h>

// Android includes
#include <android-base/stringprintf.h>
#include <base/bit_utils.h>
#include <base/globals.h>

// stdc++ includes
#include <atomic>

// GART includes
#include <gart/env.h>

// local includes
#include "mem_map.h"

class Assertion_failed : Genode::Exception { };

#define assert(invariant, message) if (!(invariant)) { Genode::error(message ": " #invariant); throw Assertion_failed(); }

static std::atomic_size_t lowmem_base = 0x1000;

namespace art {

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
                                        , prot_(prot)
                                        , reuse_(reuse)
                                        , already_unmapped_(false)
                                        , redzone_size_(redzone_size)
                                        , env_(gart::genode_env())
                                        , ram_ds_cap_(env_.ram().alloc(size))
                                        , address_space_(env_.pd().address_space())
    {
        assert ((prot & ~0x15UL) == 0, "Unsupported protection bits");
        assert (reuse == 0, "Reuse not implemented");

        bool lowmem =
#ifdef __LP64__
            (prot && PROT_LOWMEM) > 0;
#else
            false;
#endif

        if (!(prot && PROT_READ))
        {
            throw Error("Non-accessible mapping unsupported");
        }

        if ((reinterpret_cast<unsigned long>(begin) & 0xfff) != 0)
        {
            throw Error("Address not page-aligned");
        };

        Genode::Dataspace_client ds(ram_ds_cap_);
        size_ = ds.size();

        int tries = 0;
        bool done = false;
        if (lowmem && begin == nullptr)
        {
            size_t adjust = 0x1000;
            for (size_t b = lowmem_base; !done && b < (1UL << 32 - size_); b += adjust, tries += 1)
            {
                try {
                    begin_ = address_space_.attach(ram_ds_cap_, 0, 0, true, reinterpret_cast<void *>(b), prot && PROT_EXEC, prot && PROT_WRITE);
                    done = true;
                } catch (Genode::Region_map::Region_conflict) { adjust *= 2; }
            }
            if (!done)
            {
                throw Error("Error mapping low-mem");
            }
            size_t new_base = lowmem_base.load() + size_;
            lowmem_base.store(new_base);
        } else
        {
            begin_= address_space_.attach(ram_ds_cap_, 0, 0, begin != nullptr, begin, prot && PROT_EXEC, prot && PROT_WRITE);
        }

        base_begin_ = (void *)begin_;
        base_size_ = size_;
    };

    MemMap::~MemMap()
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
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
            auto result = new MemMap (/* name         */ name,
                                      /* begin        */ addr,
                                      /* size         */ byte_count,
                                      /* base_begin   */ addr,
                                      /* base_size    */ byte_count,
                                      /* prot         */ prot || (low_4gb ? PROT_LOWMEM : 0),
                                      /* reuse        */ reuse,
                                      /* redzone_size */ 0);

#ifdef __LP64__
            assert (!low_4gb
                   || (reinterpret_cast<unsigned long>(result->BaseBegin()) < (1UL << 32)
                      && reinterpret_cast<unsigned long>(result->BaseEnd()) < (1UL << 32)),
                    "low_4gb not honored");
#endif

            return result;
        } catch (Error e)
        {
            Genode::error("ErroR: ", error_msg->c_str());
            e.message(error_msg);
            return nullptr;
        } catch (...)
        {
            Genode::error(__func__, ": Unknown error");
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
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
    }

    void MemMap::Shutdown()
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
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
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        return false;
    }

#define PointerDiff(a, b) (size_t)(reinterpret_cast<uintptr_t>(a) - reinterpret_cast<uintptr_t>(b))

    void MemMap::SetSize(size_t new_size)
    {
        size_t new_base_size = RoundUp(new_size + PointerDiff(begin_, base_begin_), kPageSize);
        if (new_base_size == base_size_)
        {
            size_ = new_size;
            return;
        }

        address_space_.detach(Genode::Region_map::Local_addr(base_begin_));
        Genode::Region_map::Local_addr result = address_space_.attach(/* ds */ ram_ds_cap_,
                                                                      /* size */ new_base_size,
                                                                      /* offset */ 0,
                                                                      /* use_local_addr */ true,
                                                                      /* local_addr */ begin_,
                                                                      /* executable */ prot_ && PROT_EXEC,
                                                                      /* writeable */ prot_ && PROT_WRITE);
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
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        return nullptr;
    }

    void MemMap::MadviseDontNeedAndZero()
    {
        if (base_begin_ == nullptr && base_size_ == 0)
        {
            return;
        }

        if (prot_ && PROT_WRITE && !kMadviseZeroes)
        {
            Genode::memset(base_begin_, 0, base_size_);
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
