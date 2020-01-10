ANDROID_PORT		= libcore
ANDROID_DIR			= libcore
ANDROID_SECTIONS	= /cc_library_shared[@name=libjavacore]
ANDROID_BUILDFILES  = luni/src/main/native/Android.bp ojluni/src/main/native/Android.bp NativeCode.bp
ANDROID_SOURCES     = . luni/src/main/native

ANDROID_EXCLUDE_CC += luni/src/main/native/libcore_io_Linux.cpp

include $(call select_from_repositories,lib/mk/android-lib.inc)

CC_OPT += -Wno-maybe-uninitialized
CC_OPT += -DJNI_SIGNATURE_CHECKER_DISABLE_ASSERTS
