#ifndef __GART_UNISTD_H__
#define __GART_UNISTD_H__

#include_next <unistd.h>

// Genode does not provide gettid. This is only used in liblog - replace with PID.
static pid_t gettid(void) { return getpid(); };

#include <string.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/limits.h>

#undef ALIGN

//
// Not available on Genode
//

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(exp) ({         \
    typeof (exp) _rc;                      \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif

#define lseek64(fd, off, whence) lseek(fd, off, whence)

#endif // !__GART_UNISTD_H__
