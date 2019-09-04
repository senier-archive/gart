#include <pthread.h>
#include <stdio.h>

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

pid_t pthread_gettid_np(pthread_t thread)
{
    NOT_IMPLEMENTED;
    return -1;
};

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
