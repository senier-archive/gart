ANDROID_PORT		= fdlibm
ANDROID_DIR			= fdlibm
ANDROID_SECTIONS	= /cc_library_static[@name=libfdlibm]
ANDROID_STATIC_LIB  = yes

ANDROID_EXCLUDE_OPT += -Werror
ANDROID_EXCLUDE_OPT += -Wno-sometimes-uninitialized
ANDROID_EXCLUDE_OPT += -Wno-logical-op-parentheses

include $(call select_from_repositories,lib/mk/android-lib.inc)

CC_OPT += -Wno-maybe-uninitialized
CC_OPT += -Wno-unused-but-set-variable
CC_OPT += -Wno-misleading-indentation
CC_OPT += -Wno-parentheses
