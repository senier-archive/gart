ANDROID_PORT		= libnativehelper
ANDROID_DIR			= libnativehelper
ANDROID_SECTIONS	= /cc_library[@name=libnativehelper]
ANDROID_INCLUDES	= include_jni platform_include header_only_include

ANDROID_EXCLUDE_OPT += -fvisibility=protected

include $(call select_from_repositories,lib/mk/android-lib.inc)
