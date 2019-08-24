CORE_PATH = $(call select_from_ports,gart_core)/system/core
SHARED_LIB = yes

VPATH += $(CORE_PATH)/libutils/tests
SRC_CC += Singleton_test2.cpp
CC_OPT += -DENODATA=1
LIBS += gart_libutils_tests_singleton1 libc gart_libutils gart_libcutils
CC_CXX_WARN_STRICT =
