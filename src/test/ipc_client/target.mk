TARGET = ipc_client

ANDROID_PORT	 = binder_demo
ANDROID_DIR		 = binder_demo
ANDROID_SOURCES	 = libartbase
ANDROID_SECTIONS = /cc_test[@name=ipc_client]

LIBS += gtest gart_gtest_main

include $(call select_from_repositories,lib/mk/android-prg.inc)
