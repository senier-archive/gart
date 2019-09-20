#include <base/log.h>
#include <base/exception.h>

#include <signal.h>
#include <errno.h>
#include <sched.h>

#define NOT_IMPLEMENTED Genode::warning(__func__, ": not implemented")

class Raise : Genode::Exception { };

extern "C" {

int sigwait(const sigset_t *, int *) { NOT_IMPLEMENTED; return EINVAL; }
int sched_getscheduler(pid_t) { NOT_IMPLEMENTED; errno = EINVAL; return -1; }
int sched_getparam(pid_t, struct sched_param *) { NOT_IMPLEMENTED; return -1; }

int raise(int sig) {
    switch (sig)
    {
        case SIGKILL:
            Genode::error("Program killed by raise(SIGKILL)");
            throw Raise();
        default:
            Genode::error("raise(", sig, ")");
            return -1;
    }
}

}
