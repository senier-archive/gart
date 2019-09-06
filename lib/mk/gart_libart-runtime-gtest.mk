ANDROID_PORT       = art
ANDROID_DIR        = art
ANDROID_SOURCES    = runtime
ANDROID_BUILDFILES = $(REP_DIR)/include/vixl/Android.bp Android.bp test/Android.bp build/Android.bp runtime/Android.bp
ANDROID_SECTIONS   = /art_cc_library[@name=libart-runtime-gtest]
ANDROID_STATIC_LIB = yes

ANDROID_EXCLUDE_LIBS += libartd
ANDROID_LIBS += libart

ANDROID_EXCLUDE_LIBS += libartd-compiler
ANDROID_LIBS += libart-compiler

ANDROID_EXCLUDE_LIBS += libdexfiled
ANDROID_LIBS += libdexfile

ANDROID_EXCLUDE_LIBS += libgtest
LIBS += gtest

# jni.h required
ANDROID_LIBS += libnativehelper

# dlmalloc required
ANDROID_LIBS += dlmalloc

# unicode required
ANDROID_LIBS += libicuuc

# We want to use our own main
CC_OPT_common_runtime_test += -Dmain=__unused_main

CC_OPT += -DART_TARGET_NATIVETEST_DIR="/test"

include $(REP_DIR)/lib/mk/art_opts.inc
include $(call select_from_repositories,lib/mk/android-lib.inc)
