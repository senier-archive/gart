ANDROID_PORT		= core
ANDROID_DIR			= system/core/libziparchive
ANDROID_SECTIONS	= \
	/cc_library[@name=libziparchive] \
	/cc_library[@name=libziparchive]/target/android
ANDROID_EXCLUDE_OPT = -Wold-style-cast

include $(call select_from_repositories,lib/mk/android-lib.inc)
