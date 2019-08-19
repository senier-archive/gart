#ifndef __GART_UNISTD_H__
#define __GART_UNISTD_H__

#include_next <unistd.h>

// Genode does not provide gettid. This is only used in liblog - replace with PID.
static pid_t gettid(void) { return getpid(); };

#include <string.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/limits.h>

#endif // !__GART_UNISTD_H__
