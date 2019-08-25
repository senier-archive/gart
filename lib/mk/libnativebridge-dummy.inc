LIBNATIVEBRIDGE_TESTS_DIR = $(call select_from_ports,gart_core)/system/core/libnativebridge/tests

vpath %.cpp $(LIBNATIVEBRIDGE_TESTS_DIR)
INC_DIR += $(LIBNATIVEBRIDGE_TESTS_DIR)

LIBS += libc gart_libnativebridge gart_libnativehelper
SHARED_LIB = yes
CC_CXX_WARN_STRICT =
