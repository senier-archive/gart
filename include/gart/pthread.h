#pragma once

#include <stdio.h>
#include_next <pthread.h>

static pid_t pthread_gettid_np(pthread_t thread) {
   fprintf(stderr, "%s NOT IMPLEMENTED", __func__);
   return -1;
};

static int pthread_setname_np(pthread_t thread, const char *name) {
   fprintf(stderr, "%s NOT IMPLEMENTED (%s)", __func__, name);
   return -1;
}
static int pthread_getname_np(pthread_t thread, char *name, size_t len) {
   fprintf(stderr, "%s NOT IMPLEMENTED", __func__);
   return -1;
}

static int pthread_getattr_np(pthread_t thread, pthread_attr_t *attrs) {
   fprintf(stderr, "%s NOT IMPLEMENTED", __func__);
   return -1;
}
