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

CC_OPT = -g -Wall -Werror -fno-builtin -D__GENODE__ -D__ANDROID__

LIBLOG_TESTS_DIR := $(call select_from_ports,gart_core)/system/core/liblog/tests
vpath %.cpp $(LIBLOG_TESTS_DIR)

LIBS += libc stdcxx gtest gart_liblog gart_libcore_includes gart_libbase
