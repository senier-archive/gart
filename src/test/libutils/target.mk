TARGET = libutils_test

ANDROID_PORT	 = core
ANDROID_DIR		 = system/core/libutils/tests
ANDROID_SECTIONS = \
	/cc_test[@name=libutils_tests] \
	/cc_test[@name=libutils_tests]/target/android \

# Unsupported Clang option
ANDROID_EXCLUDE_OPT = -Wthread-safety

CC_CXX_WARN_STRICT =
CC_OPT += -Wno-ignored-qualifiers -Wno-maybe-uninitialized
LIBS += gtest gart_gtest_main

include $(call select_from_repositories,lib/mk/android-prg.inc)
