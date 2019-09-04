#include <base/log.h>
#include <pthread.h>

#define NOT_IMPLEMENTED Genode::warning(__func__, ": not implemented")

int pthread_rwlock_timedwrlock(pthread_rwlock_t *, const struct timespec *) { NOT_IMPLEMENTED; return -1; }

int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize)
{
    int rv;
    rv = pthread_attr_setstackaddr(attr, stackaddr);
    if (rv != 0) {
        return rv;
    }

    return pthread_attr_setstacksize(attr, stacksize);
}
