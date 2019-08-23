TARGET = libz_test

ANDROID_PORT	 = libz
ANDROID_DIR		 = zlib
ANDROID_SECTIONS = /cc_binary[@name=zlib_example]
LIBS += gart_main

# Run only non-file tests
CC_OPT += -DZ_SOLO=1

include $(call select_from_repositories,lib/mk/android-prg.inc)
