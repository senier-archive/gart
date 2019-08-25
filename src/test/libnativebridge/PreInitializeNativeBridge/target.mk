TARGET = test_PreInitializeNativeBridge
SRC_CC = PreInitializeNativeBridge_test.cpp

CC_OPT = -D__ANDROID__
include $(REP_DIR)/src/test/libnativebridge/target.inc
