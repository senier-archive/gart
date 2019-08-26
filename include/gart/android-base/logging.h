#pragma once

#include_next <android-base/logging.h>

#undef DCHECK_NE
#define DCHECK_NE(x, y)

#undef WOULD_LOG
#define WOULD_LOG(severity) MUST_LOG_MESSAGE(severity)
