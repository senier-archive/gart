#pragma once

#include_next <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

pid_t gettid(void);

#ifdef __cplusplus
}
#endif
