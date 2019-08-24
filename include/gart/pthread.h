#pragma once

#include <stdio.h>
#include_next <pthread.h>

static pid_t pthread_gettid_np(pthread_t thread) {
   fprintf(stderr, "%s NOT IMPLEMENTED", __func__);
   return -1;
};
