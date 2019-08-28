ANDROID_PORT		= icu
ANDROID_DIR			= icu/icu4c/source/common
ANDROID_SECTIONS	= /cc_library[@name=libicuuc]

include $(call select_from_repositories,lib/mk/android-lib.inc)

# Compile for generic platform instead of Android
CC_OPT += -U__ANDROID__

CC_OPT += -Wno-stringop-truncation -Wno-array-bounds -Wno-stringop-overflow -Wno-register
