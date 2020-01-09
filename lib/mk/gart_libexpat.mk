ANDROID_PORT		= expat
ANDROID_DIR			= expat
ANDROID_SECTIONS	= /cc_library[@name=libexpat]

CC_OPT += -DXML_DEV_URANDOM

include $(call select_from_repositories,lib/mk/android-lib.inc)
