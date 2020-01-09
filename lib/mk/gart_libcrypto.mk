ANDROID_PORT		= boringssl
ANDROID_DIR			= boringssl
ANDROID_SECTIONS	= /cc_library[@name=libcrypto] /cc_library[@name=libcrypto]/target/linux_$(call android_arch)
ANDROID_BUILDFILES	= sources.bp Android.bp

# Some files cannot be built with clang and are treated specially.
# No problem for us, as we use gcc.
ANDROID_UNDEFINED = libcrypto_no_clang

ifeq ($(call android_arch),arm)
SRC_S += linux-arm/crypto/fipsmodule/aes-armv4.S
SRC_S += linux-arm/crypto/fipsmodule/bsaes-armv7.S
endif

include $(call select_from_repositories,lib/mk/android-lib.inc)
