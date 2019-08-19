#ifndef __GART_SIGNAL_H__
#define __GART_SIGNAL_H__

#include <sys/signal.h>

static int sigemptyset(sigset_t *set) { return -1; };
static int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) { return -1; };

#endif // !__GART_SIGNAL_H__
