#pragma once

// Pretend we're Apple to disable timed rwlocks and futexes

#define __APPLE__
#include_next <base/mutex.h>
#undef __APPLE__
