#pragma once

#include <stdio.h>
#include_next <pthread.h>

static pid_t pthread_gettid_np(pthread_t thread) {
   fprintf(stderr, "%s NOT IMPLEMENTED", __func__);
   return -1;
};

static void pthread_setname_np(const char *name) {
   fprintf(stderr, "%s NOT IMPLEMENTED (%s)", __func__, name);
}
