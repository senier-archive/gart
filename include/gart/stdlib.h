#pragma once

#ifdef GART_ABORT_MACRO
#include <stdio.h>
#define abort gart_abort
#endif

#include_next <stdlib.h>

#ifdef GART_ABORT_MACRO
#undef abort
#include <gart_abort.h>
#endif
