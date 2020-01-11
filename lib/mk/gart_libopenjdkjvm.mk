ANDROID_PORT       = art
ANDROID_DIR        = art
ANDROID_SOURCES    = openjdkjvm
ANDROID_INCLUDES   = openjdkjvm
ANDROID_BUILDFILES = Android.bp test/Android.bp build/Android.bp openjdkjvm/Android.bp
ANDROID_SECTIONS   = /art_cc_library[@name=libopenjdkjvm]

ANDROID_LIBS += libcore
ANDROID_LIBS += libartbase
ANDROID_LIBS += libdexfile
ANDROID_LIBS += dlmalloc
ANDROID_LIBS += libnativehelper

include $(REP_DIR)/lib/mk/art_opts.inc
include $(call select_from_repositories,lib/mk/android-lib.inc)
