ANDROID_PORT       = art
ANDROID_DIR        = art
ANDROID_SOURCES    = compiler
ANDROID_INCLUDES	 = compiler disassembler
ANDROID_BUILDFILES = Android.bp test/Android.bp build/Android.bp compiler/Android.bp dex2oat/Android.bp
ANDROID_SECTIONS   = \
	/art_cc_library[@name=libart-compiler] \
	/art_cc_library[@name=libart-compiler]/codegen/arm \
	/art_cc_library[@name=libart-compiler]/codegen/arm64 \
	/art_cc_library[@name=libart-compiler]/codegen/x86 \
	/art_cc_library[@name=libart-compiler]/codegen/x86_64

# jni.h required
LIBS += gart_libnativehelper

# dlmalloc required
LIBS += gart_dlmalloc

# vixl is missing from blueprint
LIBS += gart_libvixl-arm
LIBS += gart_libvixl-arm64

CC_OPT += -Wno-tautological-compare
CC_OPT += -Wno-switch-unreachable

include $(REP_DIR)/lib/mk/art_opts.inc
include $(call select_from_repositories,lib/mk/android-lib.inc)
