TARGET = libart-compiler_test

ANDROID_PORT	    = art
ANDROID_DIR		    = art
ANDROID_BUILDFILES = $(REP_DIR)/include/vixl/Android.bp build/Android.bp test/Android.bp compiler/Android.bp
ANDROID_SOURCES	 = compiler
ANDROID_INCLUDES	 = compiler
ANDROID_SECTIONS   = /art_cc_test[@name=art_compiler_tests]

ANDROID_EXCLUDE_LIBS += libartd
ANDROID_LIBS += libart

ANDROID_EXCLUDE_LIBS += libartd-compiler
ANDROID_LIBS += libart-compiler

ANDROID_EXCLUDE_LIBS += libartd-disassembler
ANDROID_LIBS += libart-disassembler

ANDROID_EXCLUDE_LIBS += libartd-simulator-container
ANDROID_LIBS += libart-simulator-container

ANDROID_EXCLUDE_LIBS += libdexfiled
ANDROID_LIBS += libdexfile

ANDROID_EXCLUDE_LIBS += libvixld-arm
ANDROID_LIBS += libvixl-arm

ANDROID_EXCLUDE_LIBS += libvixld-arm64
ANDROID_LIBS += libvixl-arm64

LIBS += gtest gart_gtest_main

ANDROID_EXCLUDE_OPT += -Wno-frame-larger-than=
CC_OPT += -Wno-tautological-compare

ANDROID_EXCLUDE_OPT += -DVIXL_DEBUG

# dlmalloc required
ANDROID_LIBS += dlmalloc

# FIXME: gcc-related Compiler error
ANDROID_EXCLUDE_CC += utils/intrusive_forward_list_test.cc

include $(call select_from_repositories,lib/mk/android-prg.inc)
