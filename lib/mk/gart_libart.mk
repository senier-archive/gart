ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp runtime/Android.bp
ANDROID_INCLUDES   = runtime runtime/generated cmdline
ANDROID_SOURCES	 = runtime
ANDROID_SECTIONS	= \
	/art_cc_library[@name=libart] \
	/art_cc_library[@name=libart]/shared \
	/art_cc_library[@name=libart]/target/android \
	/art_cc_library[@name=libart]/target/android_$(call android_arch) \
	/art_cc_library[@name=libart]/arch/$(call android_arch) \

GENSRCS_SECTION=art_operator_srcs
GENSRCS_SUBDIR=runtime

# jni.h required
LIBS += gart_libnativehelper

# dlmalloc required
LIBS += gart_dlmalloc

# sigchain lib required
LIBS += gart_libsigchain

# unicode required
LIBS += gart_libicuuc

# Remove libtombstone (mocked)
ANDROID_EXCLUDE_LIBS += libtombstoned_client

# attributes are not allowed on a function-definition with gcc
CC_OPT_entrypoints/quick/quick_dexcache_entrypoints += -D'__attribute__(x)='

# Custom implementations
VPATH += $(REP_DIR)/src/libart
SRC_CC += pthread.cc libc.cc

include $(call select_from_repositories,lib/mk/android-lib.inc)
