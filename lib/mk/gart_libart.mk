ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp runtime/Android.bp
ANDROID_INCLUDES   = runtime
ANDROID_SOURCES	 = runtime
ANDROID_SECTIONS	= \
	/art_cc_library[@name=libart] \
	/art_cc_library[@name=libart]/shared \
	/art_cc_library[@name=libart]/target/android \
	/art_cc_library[@name=libart]/target/android_$(call android_arch)

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

# jni.h required
LIBS += gart_libnativehelper

# dlmalloc required
LIBS += gart_dlmalloc

# sigchain lib required
LIBS += gart_libsigchain

# Disable some logging macros to ensure constexpr can be used (cf. include/gart/android-base/logging.h)
CC_OPT += -DART_LOG_CONSTEXPR_HACK

# Remove libtombstone and libbacktrace (mocked)
ANDROID_EXCLUDE_LIBS += libtombstoned_client libbacktrace

# ART parameters (taken from build/art.go)
CC_OPT += -DART_DEFAULT_GC_TYPE_IS_CMS
CC_OPT += -DIMT_SIZE=43
CC_OPT += -DART_USE_READ_BARRIER=1 -DART_READ_BARRIER_TYPE_IS_BAKER
CC_OPT += -DART_DEFAULT_COMPACT_DEX_LEVEL=fast
CC_OPT += -DART_STACK_OVERFLOW_GAP_arm=8192
CC_OPT += -DART_STACK_OVERFLOW_GAP_arm64=8192
CC_OPT += -DART_STACK_OVERFLOW_GAP_mips=16384
CC_OPT += -DART_STACK_OVERFLOW_GAP_mips64=16384
CC_OPT += -DART_STACK_OVERFLOW_GAP_x86=8192
CC_OPT += -DART_STACK_OVERFLOW_GAP_x86_64=8192
CC_OPT += -DART_FRAME_SIZE_LIMIT=1736
CC_OPT += -DART_TARGTE -DART_TARGET_LINUX
CC_OPT += -DART_BASE_ADDRESS_MIN_DELTA=-0x1000000
CC_OPT += -DART_BASE_ADDRESS_MAX_DELTA=0x1000000
CC_OPT += -DART_USE_FUTEXES=0

# Base address (derived from build/soong/android/config.go)
CC_OPT += -DART_BASE_ADDRESS=0x70000000

# attributes are not allowed on a function-definition with gcc
CC_OPT_entrypoints/quick/quick_dexcache_entrypoints += -D'__attribute__(x)='

include $(call select_from_repositories,lib/mk/android-lib.inc)
