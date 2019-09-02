TARGET = libdexfile_test

ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp libdexfile/Android.bp
ANDROID_SECTIONS = \
	/art_cc_test[@name=art_libdexfile_tests] \
	/art_cc_test[@name=art_libdexfile_tests]/target/android

ANDROID_EXCLUDE_LIBS += libart-gtest
LIBS += gtest

LIBS += gart_gtest_main

include $(call select_from_repositories,lib/mk/android-prg.inc)
