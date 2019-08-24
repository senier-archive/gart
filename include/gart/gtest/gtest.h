#pragma once

#include <base/log.h>
#include_next <gtest/gtest.h>

#define ASSERT_DEATH(x, y) GTEST_FATAL_FAILURE_("ASSERT_DEATH not implemented")
#define EXPECT_DEATH(x, y) GTEST_FATAL_FAILURE_("EXPECT_DEATH not implemented")
