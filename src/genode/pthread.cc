// Genode includes
#include <base/log.h>

// libc includes
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// stdcxx includes
#include <mutex>

#define NOT_IMPLEMENTED fprintf(stderr, "%s NOT IMPLEMENTED", __func__);

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

enum { TIDDB_SIZE = 100 };
pthread_t *tiddb = nullptr;
std::mutex tiddb_lock;

pid_t pthread_gettid_np(pthread_t thread)
{
    std::lock_guard<std::mutex> mu(tiddb_lock);
    if (tiddb == nullptr) {
        tiddb = reinterpret_cast<pthread_t *>(malloc(TIDDB_SIZE * sizeof(pthread_t)));
        if (tiddb == nullptr) {
            return -1;
        }
    }

    for (pid_t p = 0; p < TIDDB_SIZE; p++) {
        if (tiddb[p] == nullptr) {
            tiddb[p] = thread;
            return p;
        }
        if (tiddb[p] == thread) {
            return p;
        }
    }

    Genode::error("Out of TID DB entries (TIDDB_SIZE=", int(TIDDB_SIZE), ")");
    return -1;
};

extern "C"
pid_t gettid(void) {
    return pthread_gettid_np(pthread_self());
}

int pthread_setname_np(pthread_t thread, const char *name)
{
    NOT_IMPLEMENTED;
    return -1;
}
int pthread_getname_np(pthread_t thread, char *name, size_t len)
{
    NOT_IMPLEMENTED;
    return -1;
}

int pthread_getattr_np(pthread_t thread, pthread_attr_t *attrs)
{
    NOT_IMPLEMENTED;
    return -1;
}
