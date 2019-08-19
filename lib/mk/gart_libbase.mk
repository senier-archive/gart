ANDROID_PORT	 = core
ANDROID_DIR  	 = system/core/base
ANDROID_SECTIONS = /cc_library[@name=libbase]

include $(call select_from_repositories,lib/mk/android-lib.inc)
CC_OPT += -Wno-write-strings
