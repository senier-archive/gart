ANDROID_PORT		= native
ANDROID_DIR			= native/libs/binder
ANDROID_SECTIONS	= /cc_library[@name=libbinder]

ANDROID_EXCLUDE_LIBS += libutilscallstack

include $(call select_from_repositories,lib/mk/android-lib.inc)

CC_OPT += -Wno-class-memaccess
CC_OPT += -Wno-format

LIBS += gneiss_cpp
