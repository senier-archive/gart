#include <base/log.h>
#include "mem_map.h"

namespace art {

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
        Genode::warning(__PRETTY_FUNCTION__, ": not implemented");
        Genode::warning("    name=", name);
        Genode::warning("    addr=", Genode::Hex((unsigned long long)addr));
        Genode::warning("    count=", byte_count);
        Genode::warning("    prot=", prot);
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
