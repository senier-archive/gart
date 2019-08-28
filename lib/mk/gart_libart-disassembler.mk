ANDROID_PORT       = art
ANDROID_DIR        = art
ANDROID_SOURCES    = disassembler
ANDROID_INCLUDES	 = runtime libartbase
ANDROID_BUILDFILES = build/Android.bp disassembler/Android.bp
ANDROID_SECTIONS   = /art_cc_library[@name=libart-disassembler]

ANDROID_EXCLUDE_OPT += -Wthread-safety
ANDROID_EXCLUDE_OPT += -Wint-to-void-pointer-cast
ANDROID_EXCLUDE_OPT += -Wthread-safety-negative
ANDROID_EXCLUDE_OPT += -Wunreachable-code-break
ANDROID_EXCLUDE_OPT += -Wunreachable-code-return
ANDROID_EXCLUDE_OPT += -Wused-but-marked-unused
ANDROID_EXCLUDE_OPT += -Wshadow
ANDROID_EXCLUDE_OPT += -Wfloat-equal
ANDROID_EXCLUDE_OPT += -Wimplicit-fallthrough
ANDROID_EXCLUDE_OPT += -fvisibility=protected

include $(call select_from_repositories,lib/mk/android-lib.inc)

INC_DIR += $(call select_from_ports,gart_vixl)/vixl/src
