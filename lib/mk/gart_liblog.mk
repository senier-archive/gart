ANDROID_PORT       = core
ANDROID_DIR        = system/core/liblog
ANDROID_INCLUDES   = ../include
ANDROID_SECTIONS   = /cc_library[@name=liblog] /cc_library[@name=liblog]/target/android

ANDROID_EXCLUDE_C  = logd_writer.c

include $(call select_from_repositories,lib/mk/android-lib.inc)

VPATH += $(REP_DIR)/src/liblog
SRC_C += genode_logd_writer.c
CC_OPT += -Wno-unused-variable -Wno-unused-function -Wno-array-bounds -Wno-maybe-uninitialized

INC_DIR += $(call select_from_ports,gart_core)/system/core
