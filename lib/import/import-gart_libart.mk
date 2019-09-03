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
ANDROID_EXCLUDE_OPT += -Wunused
ANDROID_EXCLUDE_OPT += -Wfloat-equal
ANDROID_EXCLUDE_OPT += -Wextra

CC_OPT += -Wno-parentheses
CC_OPT += -Wno-maybe-uninitialized
CC_OPT += -Wno-implicit-fallthrough
CC_OPT += -Wno-type-limits
CC_OPT += -Wno-comment
CC_OPT += -Wno-class-memaccess
CC_OPT += -Wno-pragmas
CC_OPT += -Wno-unknown-pragmas
CC_OPT += -Wno-unused-but-set-variable
CC_OPT += -Wno-attributes
CC_OPT += -Wno-format
CC_OPT += -Wno-return-type
CC_OPT += -Wno-overflow
CC_OPT += -Wno-int-in-bool-context
CC_OPT += -Wno-sign-compare
CC_OPT += -Wno-uninitialized
CC_OPT += -Wno-ignored-attributes

# Disable some logging macros to ensure constexpr can be used (cf. include/gart/android-base/logging.h)
CC_OPT += -DART_LOG_CONSTEXPR_HACK

# Make X_ASSERT usable in constexpr
CC_OPT += -DJNI_SIGNATURE_CHECKER_DISABLE_ASSERTS

# jni.h required
LIBS += gart_libnativehelper

# dlmalloc required
LIBS += gart_dlmalloc

include $(REP_DIR)/lib/mk/art_opts.inc

ANDROID_LIB = libart
include $(call select_from_repositories,lib/import/import-android.inc)
