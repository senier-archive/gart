ANDROID_PORT		= core
ANDROID_DIR			= system/core/libnativebridge
ANDROID_SECTIONS	= /cc_library[@name=libnativebridge]
ANDROID_LIBS      = libnativehelper

include $(call select_from_repositories,lib/mk/android-lib.inc)

# Pretend we're on apple, as this disable the use of bound mounts
CC_OPT_native_bridge += -D__APPLE__=1 -Wno-return-type
