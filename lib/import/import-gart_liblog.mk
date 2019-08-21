ANDROID_LIB = liblog
include $(call select_from_repositories,lib/import/import-android.inc)
CC_OPT += -D_USING_LIBCXX
