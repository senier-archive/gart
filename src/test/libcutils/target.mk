TARGET = libcutils_test

ANDROID_PORT	 = core
ANDROID_DIR		 = system/core/libcutils/tests
ANDROID_SECTIONS = \
	/cc_test[@name=libcutils_test] \
	/cc_test[@name=libcutils_test]/target/android

# Tests mocked Ashmem interfaces and missing mmap() functionality. Add once there are real implementations.
ANDROID_EXCLUDE_CC += AshmemTest.cpp

# Tests file system layout which we don't want to emulate completely
ANDROID_EXCLUDE_CC += fs_config.cpp

# Tests mocked scheduling policy API
ANDROID_EXCLUDE_CC += sched_policy_test.cpp

include $(call select_from_repositories,lib/mk/android-prg.inc)

LIBS += gtest gart_gtest_main
