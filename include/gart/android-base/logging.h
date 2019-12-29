#pragma once

#include_next <android-base/logging.h>
#include <gart/log_discard.h>
#include <iostream>

#ifdef ART_LOG_CONSTEXPR_HACK

#undef CHECK_OP
#define CHECK_OP(LHS, RHS, OP) if (!((LHS) OP (RHS))) { std::cout << "Check failed at " << __FILE__ << " +" << __LINE__ << std::endl; abort(); }; Discard()

#undef WOULD_LOG
#define WOULD_LOG(severity) MUST_LOG_MESSAGE(severity)

#undef LOG
#define LOG(severity) std::cerr << std::endl << "GART_" << #severity << ": "

#endif // ART_LOG_CONSTEXPR_HACK
