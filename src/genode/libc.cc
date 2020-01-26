#include <base/log.h>

#include <signal.h>
#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/auxv.h>

#define NOT_IMPLEMENTED Genode::warning(__func__, ": not implemented")

class Raise : Genode::Exception { };

extern "C" {

int sigwait(const sigset_t *, int *) { NOT_IMPLEMENTED; return EINVAL; }
int sched_getscheduler(pid_t) { NOT_IMPLEMENTED; errno = EINVAL; return -1; }
int sched_getparam(pid_t, struct sched_param *) { NOT_IMPLEMENTED; return -1; }

int futimes (int fd, const struct timeval tv[2]) {
	errno = ENOSYS;
	return -1;
}

int lchown (const char *pathname, uid_t owner, gid_t group) {
	errno = ENOSYS;
	return -1;
}

int mincore (void *addr, size_t length, unsigned char *vec) {
	errno = ENOSYS;
	return -1;
}

unsigned long getauxval(unsigned long type) {
	return 0;
}

}
