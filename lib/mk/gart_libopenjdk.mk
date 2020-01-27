ANDROID_PORT		= libcore
ANDROID_DIR			= libcore
ANDROID_SECTIONS	= /cc_library_shared[@name=libopenjdk]
ANDROID_BUILDFILES  = luni/src/main/native/Android.bp ojluni/src/main/native/Android.bp NativeCode.bp
ANDROID_SOURCES     = . luni/src/main/native ojluni/src/main/native

ANDROID_LIBS += libfdlibm

ANDROID_EXCLUDE_C += EPoll.c
ANDROID_EXCLUDE_C += LinuxWatchService.c
ANDROID_EXCLUDE_C += LinuxNativeDispatcher.c

ANDROID_EXCLUDE_OPT += -Wno-sometimes-uninitialized
ANDROID_EXCLUDE_OPT += -Wno-parentheses-equality
ANDROID_EXCLUDE_OPT += -Wno-constant-logical-operand

include $(call select_from_repositories,lib/mk/android-lib.inc)

CC_OPT += -Wno-maybe-uninitialized
CC_OPT += -Wno-uninitialized
CC_OPT += -Wno-stringop-truncation
CC_OPT += -Wno-unused-but-set-variable

CC_OPT += -DJNI_SIGNATURE_CHECKER_DISABLE_ASSERTS
CC_OPT += -D__SIGRTMIN=SIGRTMIN
CC_OPT += -D_ALLBSD_SOURCE -DGART_BSD_COMPAT

CC_OPT_io_util_md = -UGART_BSD_COMPAT -Dstat64=stat

CC_OPT_NativeThread += -Wno-int-to-pointer-cast
CC_OPT_NativeThread += -Wno-pointer-to-int-cast
