ANDROID_EXCLUDE_OPT += -Wthread-safety
ANDROID_EXCLUDE_OPT += -Wint-to-void-pointer-cast
ANDROID_EXCLUDE_OPT += -Wthread-safety-negative
ANDROID_EXCLUDE_OPT += -Wunreachable-code-break
ANDROID_EXCLUDE_OPT += -Wunreachable-code-return
ANDROID_EXCLUDE_OPT += -Wused-but-marked-unused
ANDROID_EXCLUDE_OPT += -Wshadow
ANDROID_EXCLUDE_OPT += -Wfloat-equal
ANDROID_EXCLUDE_OPT += -Wimplicit-fallthrough
ANDROID_EXCLUDE_OPT += -fvisibility=protected

ANDROID_LIB = libart-compiler
include $(call select_from_repositories,lib/import/import-android.inc)
