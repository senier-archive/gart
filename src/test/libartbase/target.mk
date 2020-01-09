TARGET = libartbase_test

ANDROID_PORT	    = art
ANDROID_DIR		    = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp libartbase/Android.bp
ANDROID_SOURCES	 = libartbase
ANDROID_INCLUDES	 = libartbase
ANDROID_SECTIONS   = /art_cc_test[@name=art_libartbase_tests]

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

LIBS += gtest gart_gtest_main

ANDROID_EXCLUDE_OPT += -Wno-frame-larger-than=
ANDROID_EXCLUDE_OPT += -DVIXL_DEBUG

include $(call select_from_repositories,lib/mk/android-prg.inc)
