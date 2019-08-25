TARGET = liblz4_test

LIBLZ4_PROGRAMS_DIR = $(call select_from_ports,gart_liblz4)/lz4/programs
LIBLZ4_TESTS_DIR = $(call select_from_ports,gart_liblz4)/lz4/tests

VPATH += $(LIBLZ4_TESTS_DIR)
INC_DIR += $(LIBLZ4_PROGRAMS_DIR)

SRC_CC += main.cc fuzzer.c
LIBS   += libc stdcxx gart_liblz4
CC_CXX_WARN_STRICT =
