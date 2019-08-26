ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp libdexfile/Android.bp
ANDROID_SOURCES	 = libdexfile
ANDROID_SECTIONS	= \
	/art_cc_library[@name=libdexfile] \
	/art_cc_library[@name=libdexfile]/target/android

ANDROID_INCLUDES += libartbase libdexfile

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

# jni.h required
LIBS += gart_libnativehelper

# Build ART for Android
CC_OPT += -DART_TARGET_ANDROID

include $(call select_from_repositories,lib/mk/android-lib.inc)
