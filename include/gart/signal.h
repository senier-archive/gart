#ifndef __GART_SIGNAL_H__
#define __GART_SIGNAL_H__

#include_next <signal.h>
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void raise_SIGINT();

#ifdef __cplusplus
}
#endif

#define raise(sig) raise_##sig()

#endif // !__GART_SIGNAL_H__
