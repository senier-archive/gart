ANDROID_PORT		 = vixl
ANDROID_DIR			 = vixl
ANDROID_INCLUDES	 = src
ANDROID_BUILDFILES = $(REP_DIR)/include/vixl/Android.bp Android.bp
ANDROID_SECTIONS	 = /cc_library[@name=libvixl-arm]

ANDROID_EXCLUDE_OPT += -pedantic

include $(call select_from_repositories,lib/mk/android-lib.inc)

SRC_CC = \
	src/code-buffer-vixl.cc \
	src/compiler-intrinsics-vixl.cc \
	src/utils-vixl.cc \
	src/aarch32/assembler-aarch32.cc \
	src/aarch32/constants-aarch32.cc \
	src/aarch32/disasm-aarch32.cc \
	src/aarch32/instructions-aarch32.cc \
	src/aarch32/location-aarch32.cc \
	src/aarch32/macro-assembler-aarch32.cc \
	src/aarch32/operands-aarch32.cc

