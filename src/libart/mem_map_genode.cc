#include <base/log.h>
#include <base/exception.h>

#include <gart/env.h>
#include <android-base/stringprintf.h>
#include "mem_map.h"

class Assertion_failed : Genode::Exception { };

#define assert(invariant, message) if (!(invariant)) { Genode::error(message ": " #invariant); throw Assertion_failed(); }

namespace art {

    enum { PROT_READ = 1, PROT_WRITE = 2, PROT_EXEC = 4 };

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
        Genode::Env &_env = gart::genode_env();

        assert ((prot & ~0x7UL) == 0, "Unsupported protection bits");
        assert (reuse == 0, "Reuse not implemented");

        if ((reinterpret_cast<unsigned long>(addr) & 0xfff) != 0)
        {
            if (error_msg != nullptr)
            {
                *error_msg = android::base::StringPrintf("Address not page-aligned");
            }
            return nullptr;
        };


        bool prot_read  = prot && PROT_READ;
        bool prot_write = prot && PROT_WRITE;
        bool prot_exec  = prot && PROT_EXEC;

        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        Genode::warning("    name=", name);
        Genode::warning("    addr=", Genode::Hex((unsigned long long)addr));
        Genode::warning("    count=", byte_count);
        Genode::warning("    read=", prot_read);
        Genode::warning("    write=", prot_write);
        Genode::warning("    exec=", prot_exec);
        Genode::warning("    low_4gb=", low_4gb);
        Genode::warning("    reuse=", reuse);
        Genode::warning("    error_msg=", error_msg != nullptr);
        Genode::warning("    use_ashmem=", use_ashmem);
        return nullptr;
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

    void MemMap::SetSize(size_t new_size)
    {
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
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
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
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
