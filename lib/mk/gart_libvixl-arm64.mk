ANDROID_PORT		 = vixl
ANDROID_DIR			 = vixl
ANDROID_BUILDFILES = $(REP_DIR)/include/vixl/Android.bp Android.bp
ANDROID_SECTIONS	 = /cc_library[@name=libvixl-arm]

ANDROID_EXCLUDE_OPT = -Wimplicit-fallthrough
CC_OPT += -Wno-stringop-truncation
CC_OPT += -Wno-implicit-fallthrough

include $(call select_from_repositories,lib/mk/android-lib.inc)

SRC_CC = \
	src/code-buffer-vixl.cc \
	src/compiler-intrinsics-vixl.cc \
	src/utils-vixl.cc \
	src/aarch64/assembler-aarch64.cc \
	src/aarch64/cpu-aarch64.cc \
	src/aarch64/debugger-aarch64.cc \
	src/aarch64/decoder-aarch64.cc \
	src/aarch64/disasm-aarch64.cc \
	src/aarch64/instructions-aarch64.cc \
	src/aarch64/instrument-aarch64.cc \
	src/aarch64/logic-aarch64.cc \
	src/aarch64/macro-assembler-aarch64.cc \
	src/aarch64/operands-aarch64.cc \
	src/aarch64/simulator-aarch64.cc
