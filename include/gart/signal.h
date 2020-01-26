#pragma once

#define raise(sig) unused_raise(sig)
#include_next <signal.h>
#undef raise

#include <stdlib.h>
#include <stdio.h>

#define raise(sig) fprintf(stderr, "ERROR: signal(" # sig ") raised"); abort();
