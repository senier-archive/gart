ANDROID_PORT = liblz4
ANDROID_DIR = lz4/lib
ANDROID_SECTIONS = /cc_library[@name=liblz4]

include $(call select_from_repositories,lib/mk/android-lib.inc)
