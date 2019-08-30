#pragma once

#include_next <android-base/logging.h>
#include <gart/log_discard.h>

#undef CHECK_OP
#define CHECK_OP(LHS, RHS, OP) Discard()

#ifdef ART_LOG_CONSTEXPR_HACK
#undef WOULD_LOG
#define WOULD_LOG(severity) MUST_LOG_MESSAGE(severity)
#endif // ART_LOG_CONSTEXPR_HACK
