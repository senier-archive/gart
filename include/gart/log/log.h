#ifndef __GART_LOG_LOG_H__
#define __GART_LOG_LOG_H__

#include_next <log/log.h>

// This prototype gets defined only when __linux__ is defined
clockid_t android_log_clockid(void);

#endif // !__GART_LOG_LOG_H__
