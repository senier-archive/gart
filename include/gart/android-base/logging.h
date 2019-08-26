#pragma once

#include_next <android-base/logging.h>

#undef DCHECK_NE
#define DCHECK_NE(x, y)

#ifdef ART_LOG_CONSTEXPR_HACK
#undef WOULD_LOG
#define WOULD_LOG(severity) MUST_LOG_MESSAGE(severity)
#endif // ART_LOG_CONSTEXPR_HACK
