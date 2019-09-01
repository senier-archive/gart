ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp sigchainlib/Android.bp
ANDROID_SOURCES	 = sigchainlib
ANDROID_INCLUDES	 = sigchainlib
ANDROID_SECTIONS	 = /cc_library_static[@name=libsigchain_dummy]

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

# Fix visiblility
ANDROID_EXCLUDE_OPT += -fvisibility=protected

include $(call select_from_repositories,lib/mk/android-lib.inc)
