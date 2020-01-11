#ifndef __GART_UNISTD_H__
#define __GART_UNISTD_H__

#include_next <unistd.h>

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

#ifndef GART_BSD_COMPAT

#ifndef lseek64
#define lseek64(fd, off, whence) lseek(fd, off, whence)
#endif

#ifndef ftruncate64
#define ftruncate64(fd, length) ftruncate(fd, length)
#endif

#ifndef open64
#define open64(pathname, flags, mode) open(pathname, flags, mode)
#endif

#ifndef fstat64
#define fstat64(pathname, statbuf) fstat(pathname, statbuf)
#endif

#ifndef stat64
#define stat64(pathname, statbuf) stat(pathname, statbuf)
#endif

#endif

#endif // !__GART_UNISTD_H__
