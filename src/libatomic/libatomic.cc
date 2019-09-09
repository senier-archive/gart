#include <base/lock.h>
#include <cpu/memory_barrier.h>

static Genode::Lock __atomic_16_lock;

extern "C"
__int128 unsigned __atomic_load_16 (const volatile void *ptr, int) {
    __int128 unsigned result;
    Genode::memory_barrier();
    __atomic_16_lock.lock();
    result = *static_cast<const volatile __int128 unsigned *>(ptr);
    __atomic_16_lock.unlock();
    Genode::memory_barrier();
    return result;
}

extern "C"
void __atomic_store_16 (volatile void *ptr, const __int128 unsigned value, int) {
    Genode::memory_barrier();
    __atomic_16_lock.lock();
    *static_cast<volatile __int128 unsigned  *>(ptr) = value;
    __atomic_16_lock.unlock();
    Genode::memory_barrier();
}
