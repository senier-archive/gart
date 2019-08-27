#ifndef __GART_SIGNAL_H__
#define __GART_SIGNAL_H__

#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>

static int sigemptyset(sigset_t *set) { return -1; }
static int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) { return -1; }

static
void raise_SIGINT() {
   fprintf(stderr, "raise(SIGINT) not implemented");
   exit(1);
}

#define raise(sig) raise_##sig()

#endif // !__GART_SIGNAL_H__
