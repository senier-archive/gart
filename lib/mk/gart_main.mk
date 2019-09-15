VPATH += $(REP_DIR)/src/main
LIBS  += stdcxx gart_genode
SRC_C += main.cpp
CC_CXX_WARN_STRICT =
CC_OPT += -Wno-write-strings
