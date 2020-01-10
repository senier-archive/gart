#pragma once

#include <stdio.h>

#define abort gart_abort
#include_next <stdlib.h>
#undef abort

#include <gart_abort.h>
