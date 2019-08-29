vpath %.c $(REP_DIR)/src/external/dlmalloc
INC_DIR += $(REP_DIR)/src/external/dlmalloc

LIBS += libc
SRC_C  += malloc.c
