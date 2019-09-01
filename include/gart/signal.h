#ifndef __GART_SIGNAL_H__
#define __GART_SIGNAL_H__

#include_next <signal.h>
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>

static
void raise_SIGINT() {
   fprintf(stderr, "raise(SIGINT) not implemented");
   exit(1);
}

#define raise(sig) raise_##sig()

#endif // !__GART_SIGNAL_H__
