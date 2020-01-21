TARGET = libart_test

ANDROID_PORT	    = art
ANDROID_DIR		    = art
ANDROID_BUILDFILES	= build/Android.bp test/Android.bp runtime/Android.bp
ANDROID_SOURCES		= runtime
ANDROID_INCLUDES	= runtime
ANDROID_SECTIONS	= /art_cc_test[@name=art_runtime_tests]

ANDROID_EXCLUDE_LIBS += libartd
ANDROID_LIBS += libart

ANDROID_EXCLUDE_LIBS += libartd-disassembler
ANDROID_LIBS += libart-disassembler

ANDROID_EXCLUDE_LIBS += libdexfiled
ANDROID_LIBS += libdexfile

ANDROID_EXCLUDE_LIBS += libvixld-arm
ANDROID_LIBS += libvixl-arm

ANDROID_EXCLUDE_LIBS += libvixld-arm64
ANDROID_LIBS += libvixl-arm64

ANDROID_LIBS += gtest_main dlmalloc libutils
LIBS += gtest

ANDROID_EXCLUDE_OPT += -Wno-frame-larger-than=
ANDROID_EXCLUDE_OPT += -DVIXL_DEBUG

ANDROID_EXCLUDE_CC += gc/space/rosalloc_space_random_test.cc
ANDROID_EXCLUDE_CC += gc/space/rosalloc_space_static_test.cc
ANDROID_EXCLUDE_CC += gc/space/dlmalloc_space_random_test.cc
ANDROID_EXCLUDE_CC += gc/space/dlmalloc_space_static_test.cc

include $(call select_from_repositories,lib/mk/android-prg.inc)

CC_OPT += -DGART_ABORT_MACRO
