#pragma once
#include_next <base/macros.h>

#undef PACKED
#define PACKED(x) __attribute__ ((__aligned__(x)))
