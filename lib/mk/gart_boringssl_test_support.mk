ANDROID_PORT		= boringssl
ANDROID_DIR			= boringssl
ANDROID_SECTIONS	= /cc_library_static[@name=boringssl_test_support]
ANDROID_BUILDFILES	= sources.bp Android.bp
ANDROID_STATIC_LIB  = yes

include $(call select_from_repositories,lib/mk/android-lib.inc)
