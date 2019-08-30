ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp libartbase/Android.bp
ANDROID_SOURCES	 = libartbase
ANDROID_INCLUDES	 = libartbase
ANDROID_SECTIONS	 = /art_cc_library[@name=libartbase]

# Fix visiblility
ANDROID_EXCLUDE_OPT += -fvisibility=protected

# Options unsupported by GCC
ANDROID_EXCLUDE_OPT += -Wint-to-void-pointer-cast
ANDROID_EXCLUDE_OPT += -Wthread-safety
ANDROID_EXCLUDE_OPT += -Wthread-safety-negative
ANDROID_EXCLUDE_OPT += -Wunreachable-code-break
ANDROID_EXCLUDE_OPT += -Wunreachable-code-return
ANDROID_EXCLUDE_OPT += -Wused-but-marked-unused

# Mute warnings
ANDROID_EXCLUDE_OPT += -Wdeprecated
ANDROID_EXCLUDE_OPT += -Wshadow
ANDROID_EXCLUDE_OPT += -Wredundant-decls
ANDROID_EXCLUDE_OPT += -Wimplicit-fallthrough

CC_OPT += -Wno-parentheses
CC_OPT += -Wno-maybe-uninitialized
CC_OPT += -Wno-implicit-fallthrough
CC_OPT += -Wno-type-limits
CC_OPT += -Wno-unused-but-set-variable

# Build ART for Android
CC_OPT += -DART_TARGET -DART_TARGET_LINUX

# Disable some logging macros to ensure constexpr can be used (cf. include/gart/android-base/logging.h)
CC_OPT += -DART_LOG_CONSTEXPR_HACK

# Prevent we're Linux to use clock_gettime in favor of gettimeofday
CC_OPT_base/time_utils += -D__linux__

include $(call select_from_repositories,lib/mk/android-lib.inc)
