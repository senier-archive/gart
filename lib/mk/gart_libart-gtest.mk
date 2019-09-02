ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp runtime/Android.bp
ANDROID_INCLUDES   = runtime
ANDROID_SOURCES	 = runtime
ANDROID_SECTIONS	= /art_cc_library[@name=libart-gtest]

ANDROID_EXCLUDE_LIBS += libartd
ANDROID_LIBS += libart

ANDROID_EXCLUDE_LIBS += libartd-compiler
ANDROID_LIBS += libart-compiler

ANDROID_EXCLUDE_LIBS += libdexfiled
ANDROID_LIBS += libdexfile

ANDROID_EXCLUDE_LIBS += libgtest
LIBS += gtest

include $(call select_from_repositories,lib/mk/android-lib.inc)

