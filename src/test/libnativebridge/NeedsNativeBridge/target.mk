TARGET = test_NeedsNativeBridge
SRC_CC = NeedsNativeBridge_test.cpp
include $(REP_DIR)/src/test/libnativebridge/target.inc

# For android-base/macros.h
LIBS += gart_libbase
