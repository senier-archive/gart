ANDROID_PORT     = core
ANDROID_DIR      = system/core/libnativeloader
ANDROID_SECTIONS = /cc_library[@name=libnativeloader]

# For android_dlopen_ext implementation
ANDROID_LIBS += genode

include $(call select_from_repositories,lib/mk/android-lib.inc)
