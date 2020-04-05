TARGET = ipc_client

ANDROID_PORT	 = binder_demo
ANDROID_DIR		 = binder_demo
ANDROID_SECTIONS = /cc_test[@name=ipc_client]

LIBS += gtest gart_main gart_liblog

include $(call select_from_repositories,lib/mk/android-prg.inc)
