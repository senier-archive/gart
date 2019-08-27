ANDROID_PORT		= core
ANDROID_DIR			= system/core/libutils
ANDROID_SECTIONS	= \
	/cc_library[@name=libutils] \
	/cc_library[@name=libutils]/target/android

ANDROID_EXCLUDE_OPT = -fvisibility=protected
VPATH += $(REP_DIR)/src/libutils

CC_OPT += -Wno-class-memaccess
SRC_CC += memalign.c

include $(call select_from_repositories,lib/mk/android-lib.inc)
