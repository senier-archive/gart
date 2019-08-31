#pragma once

#include_next <android-base/logging.h>
#include <gart/log_discard.h>

#ifdef ART_LOG_CONSTEXPR_HACK

#undef CHECK_OP
#define CHECK_OP(LHS, RHS, OP) Discard()

#undef WOULD_LOG
#define WOULD_LOG(severity) MUST_LOG_MESSAGE(severity)

#endif // ART_LOG_CONSTEXPR_HACK
