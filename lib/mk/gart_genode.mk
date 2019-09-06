VPATH += $(REP_DIR)/src/genode

LIBS += libc

SRC_CC += libc.cc
SRC_CC += pthread.cc
SRC_CC += dlext.cc
SRC_CC += unwind.cc

CC_OPT += -Wno-unused-parameter
INC_DIR += $(REP_DIR)/include/gart
