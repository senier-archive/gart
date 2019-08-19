TARGET = test_dup_stderr
SRC_CC += main.cc
LIBS   += libc
CC_OPT += -Wno-unused-parameter -Wno-write-strings
