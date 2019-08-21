ANDROID_PORT		= core
ANDROID_DIR			= system/core/libmetricslogger
ANDROID_SECTIONS	= /cc_library_shared[@name=libmetricslogger]


# Gtest is included in the defaults also used for the library and we do not
# want to link it.
ANDROID_EXCLUDE_LIBS = libgtest_prod

include $(call select_from_repositories,lib/mk/android-lib.inc)
