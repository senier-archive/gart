ANDROID_PORT       = art
ANDROID_DIR        = art
ANDROID_SOURCES    = compiler
ANDROID_BUILDFILES = $(REP_DIR)/include/vixl/Android.bp Android.bp test/Android.bp build/Android.bp compiler/Android.bp
ANDROID_SECTIONS   = /art_cc_library[@name=libart-compiler-gtest]

# Make this lib static to reduce number of shared libs to be put into an image
ANDROID_STATIC_LIB = yes

ANDROID_EXCLUDE_LIBS += libartd
ANDROID_LIBS += libart

ANDROID_EXCLUDE_LIBS += libartd-compiler
ANDROID_LIBS += libart-compiler

ANDROID_EXCLUDE_LIBS += libartd-disassembler
ANDROID_LIBS += libart-disassembler

ANDROID_EXCLUDE_LIBS += libdexfiled
ANDROID_LIBS += libdexfile

ANDROID_EXCLUDE_LIBS += libgtest
LIBS += gtest

# jni.h required
ANDROID_LIBS += libnativehelper

# dlmalloc required
ANDROID_LIBS += dlmalloc

include $(REP_DIR)/lib/mk/art_opts.inc
include $(call select_from_repositories,lib/mk/android-lib.inc)
