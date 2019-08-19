TARGET = liblog_test

SRC_CC = \
	main.cc \
	libc_test.cpp \
	liblog_test_local.cpp \
	log_id_test.cpp \
	log_radio_test.cpp \
	log_read_test.cpp \
	log_system_test.cpp \
	log_time_test.cpp

CC_OPT += -g -Wall -Werror -fno-builtin -D__GENODE__ -D__ANDROID__
CC_OPT += -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-dangling-else

CC_CXX_WARN_STRICT =
LIBLOG_TESTS_DIR := $(call select_from_ports,gart_core)/system/core/liblog/tests
INC_DIR := $(REP_DIR)/include/gart $(INC_DIR)
vpath %.cpp $(LIBLOG_TESTS_DIR)

LIBS += libc stdcxx gtest gart_libcore_includes gart_liblog gart_libbase
