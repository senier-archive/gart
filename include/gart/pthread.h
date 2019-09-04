#pragma once

#include <stdio.h>
#include_next <pthread.h>

pid_t pthread_gettid_np(pthread_t thread);
int pthread_setname_np(pthread_t thread, const char *name);
int pthread_getname_np(pthread_t thread, char *name, size_t len);
int pthread_getattr_np(pthread_t thread, pthread_attr_t *attrs);
