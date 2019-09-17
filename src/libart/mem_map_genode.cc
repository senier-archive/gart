// Genode includes
#include <base/log.h>
#include <base/exception.h>
#include <dataspace/client.h>
#include <util/string.h>

// Android includes
#include <android-base/stringprintf.h>
#include <base/bit_utils.h>
#include <base/globals.h>

// GART includes
#include <gart/env.h>

// local includes
#include "mem_map.h"

class Assertion_failed : Genode::Exception { };

#define assert(invariant, message) if (!(invariant)) { Genode::error(message ": " #invariant); throw Assertion_failed(); }

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

        bool lowmem = (prot && PROT_LOWMEM) > 0;

        if (!(prot && PROT_READ))
        {
            throw Error("Non-accessible mapping unsupported");
        }

        if ((reinterpret_cast<unsigned long>(begin) & 0xfff) != 0)
        {
            throw Error("Address not page-aligned");
        };

        Genode::Region_map::Local_addr result = address_space_.attach(/* ds */ ram_ds_cap_,
                                                                      /* size */ 0,
                                                                      /* offset */ 0,
                                                                      /* use_local_addr */ begin != nullptr,
                                                                      /* local_addr */ begin,
                                                                      /* executable */ prot && PROT_EXEC,
                                                                      /* writeable */ prot && PROT_WRITE);

        begin_ = result;

        Genode::Dataspace_client ds(ram_ds_cap_);
        size_ = ds.size();

        base_begin_ = (void *)begin;
        base_size_ = size_;

        Genode::log("Mapped \"", name.c_str(), "\" (", size, ") to ", Genode::Hex(result));
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

            assert ((reinterpret_cast<uintptr_t>(result->BaseBegin()) >> 32) == 0
                    && (reinterpret_cast<uintptr_t>(result->BaseEnd()) >> 32) == 0,
                    "low_4gb not honored");

            return result;
        } catch (Error e)
        {
            e.message(error_msg);
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

#define PointerDiff(a, b) reinterpret_cast<size_t>(reinterpret_cast<uintptr_t>(a) - reinterpret_cast<uintptr_t>(b))

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
        Genode::log("SetSize ", Genode::Hex(size_), " -> ", Genode::Hex(new_size),
            " (", Genode::Hex(base_size_), " -> ", Genode::Hex(new_base_size), ")");
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

        if (!kMadviseZeroes)
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
