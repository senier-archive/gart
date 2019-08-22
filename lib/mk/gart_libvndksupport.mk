ANDROID_PORT		= core
ANDROID_DIR			= system/core/libvndksupport
ANDROID_SECTIONS	= /cc_library[@name=libvndksupport]
ANDROID_EXCLUDE_CC += linker.c

VPATH += $(REP_DIR)/src/libvndksupport

ANDROID_EXCLUDE_C += linker.c
SRC_C += linker_genode.c
LIBS += libc
