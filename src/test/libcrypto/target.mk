TARGET = libcrypto_test

ANDROID_PORT		= boringssl
ANDROID_DIR			= boringssl
ANDROID_SECTIONS	= /cc_test[@name=boringssl_crypto_test]
ANDROID_BUILDFILES	= sources.bp Android.bp

LIBS += gtest gart_main

include $(call select_from_repositories,lib/mk/android-prg.inc)
