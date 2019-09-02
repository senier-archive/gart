ANDROID_PORT	  = core
ANDROID_DIR	     = system/core/libbacktrace
ANDROID_INCLUDES = include
ANDROID_SECTIONS = /cc_library_headers[@name=libbacktrace_headers]

include $(call select_from_repositories,lib/mk/android-lib.inc)
