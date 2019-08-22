ANDROID_PORT		= libz
ANDROID_DIR			= zlib
ANDROID_SECTIONS	= /cc_library[@name=libz]

include $(call select_from_repositories,lib/mk/android-lib.inc)
