TARGET = test_CodeCacheStatFail
SRC_CC = CodeCacheStatFail_test.cpp
include $(REP_DIR)/src/test/libnativebridge/target.inc

VPATH += $(REP_DIR)/src/test
SRC_C += creat.c
