ANDROID_PORT		= core
ANDROID_DIR			= system/core/libnativebridge
ANDROID_SECTIONS	= /cc_library[@name=libnativebridge]
#ANDROID_EXCLUDE_OPT += -fvisibility=protected

include $(call select_from_repositories,lib/mk/android-lib.inc)
