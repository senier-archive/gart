VPATH += $(REP_DIR)/src/gtest
LIBS  += gart_libgtest_prod stdcxx
SRC_C += gtest_main.cpp
CC_CXX_WARN_STRICT =
CC_OPT += -Wno-write-strings
