ANDROID_PORT		= icu
ANDROID_DIR			= icu/icu4c/source/i18n
ANDROID_SECTIONS	= \
	/cc_library_shared[@name=libicui18n] \
	/cc_library_shared[@name=libicui18n]/target/android

include $(call select_from_repositories,lib/mk/android-lib.inc)

# Compile for generic platform instead of Android
CC_OPT += -U__ANDROID__

CC_OPT += -Wno-stringop-truncation -Wno-array-bounds -Wno-stringop-overflow -Wno-register
CC_OPT += -Wno-deprecated-declarations -Wno-restrict
