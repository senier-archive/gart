ANDROID_PORT		= core
ANDROID_DIR			= system/core/libutils
ANDROID_SECTIONS	= \
	/cc_library[@name=libutils] \
	/cc_library[@name=libutils]/target/android

ANDROID_EXCLUDE_OPT += -fvisibility=protected
CC_OPT += -Wno-class-memaccess

include $(call select_from_repositories,lib/mk/android-lib.inc)
