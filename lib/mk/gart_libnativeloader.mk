ANDROID_PORT     = core
ANDROID_DIR      = system/core/libnativeloader
ANDROID_SECTIONS = /cc_library[@name=libnativeloader]

include $(call select_from_repositories,lib/mk/android-lib.inc)
