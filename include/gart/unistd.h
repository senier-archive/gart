#ifndef __GART_UNISTD_H__
#define __GART_UNISTD_H__

#include_next <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

pid_t gettid(void);

#ifdef __cplusplus
}
#endif

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
#define ftruncate64(fd, length) ftruncate(fd, length)

#endif // !__GART_UNISTD_H__
