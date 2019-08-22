#pragma once
#include_next <time.h>

// CLOCK_BOOTTIME is Linux specific
#define CLOCK_BOOTTIME CLOCK_MONOTONIC
