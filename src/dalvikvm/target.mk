TARGET = dalvikvm

ANDROID_PORT	    = art
ANDROID_DIR			= art
ANDROID_BUILDFILES	= build/Android.bp dalvikvm/Android.bp
ANDROID_SOURCES		= dalvikvm
ANDROID_SECTIONS	= /art_cc_binary[@name=dalvikvm] /art_cc_binary[@name=dalvikvm]/target/android

LIBS += gart_main

include $(call select_from_repositories,lib/mk/android-prg.inc)
