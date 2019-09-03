ANDROID_PORT       = art
ANDROID_DIR        = art
ANDROID_SOURCES    = simulator
ANDROID_INCLUDES	 = simulator simulator/include
ANDROID_BUILDFILES = Android.bp test/Android.bp build/Android.bp simulator/Android.bp
ANDROID_SECTIONS   = /art_cc_library[@name=libart-simulator-container]

# jni.h required
ANDROID_LIBS += libnativehelper

# dlmalloc required
ANDROID_LIBS += dlmalloc

# libartbase required
ANDROID_LIBS += libartbase

include $(REP_DIR)/lib/mk/art_opts.inc
include $(call select_from_repositories,lib/mk/android-lib.inc)
