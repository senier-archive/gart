TARGET = jni_invocation_test

ANDROID_PORT	 = libnativehelper
ANDROID_DIR		 = libnativehelper/tests
ANDROID_SECTIONS = /cc_test[@name=JniInvocation_test]

CC_CXX_WARN_STRICT =
LIBS += gtest gart_gtest_main

include $(call select_from_repositories,lib/mk/android-prg.inc)
