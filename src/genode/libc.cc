#include <base/log.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>

#define NOT_IMPLEMENTED Genode::warning(__func__, ": not implemented")

extern "C" {

int sigwait(const sigset_t *, int *) { NOT_IMPLEMENTED; return EINVAL; }
int sched_getscheduler(pid_t) { NOT_IMPLEMENTED; errno = EINVAL; return -1; }
int sched_getparam(pid_t, struct sched_param *) { NOT_IMPLEMENTED; return -1; }

void raise_SIGINT() {
   NOT_IMPLEMENTED;
   exit(1);
}

}
