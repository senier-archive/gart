TARGET = libmetricslogger_test

ANDROID_PORT	 = core
ANDROID_DIR		 = system/core/libmetricslogger
ANDROID_SECTIONS = /cc_test[@name=metricslogger_tests]

include $(call select_from_repositories,lib/mk/android-prg.inc)
