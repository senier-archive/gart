SRC_CC += main.cc
CC_OPT += -g -Wall -Werror -fno-builtin

LIBNATIVEBRIDGE_TESTS_DIR := $(call select_from_ports,gart_core)/system/core/libnativebridge/tests
vpath %.cpp $(LIBNATIVEBRIDGE_TESTS_DIR)
vpath %.cc $(REP_DIR)/src/test/libnativebridge

INC_DIR += $(LIBNATIVEBRIDGE_TESTS_DIR)

LIBS += libc stdcxx gtest gart_liblog gart_libnativehelper gart_libnativebridge #libcore_includes
CC_CXX_WARN_STRICT =
