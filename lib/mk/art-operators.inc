check_android_vars := $(call _assert,$(GENSRCS_SECTION), GENSRCS_SECTION not set for $(ANDROID_NAME))
check_android_vars := $(call _assert,$(GENSRCS_SUBDIR), GENSRCS_SUBDIR not set for $(ANDROID_NAME))

# Generate operator implementations
GENSRCS_BASEDIR = $(firstword $($(ANDROID_NAME)_BASEDIR))
GENSRCS_HEADERS = $(addprefix $(GENSRCS_BASEDIR)/$(GENSRCS_SUBDIR)/,$(filter %.h,$($(ANDROID_NAME)_SRCS)))
GENSRCS_OUTPUT  = $($(ANDROID_NAME)_OUTPUT_EXTENSION)
GENSRCS_DIR     = $(LIB_CACHE_DIR)/$(ANDROID_BUILDTYPE)/$(ANDROID_NAME)

GENSRCS_TOOL = $(call select_from_ports,gart_art)/art/tools/generate_operator_out.py

VPATH += $(GENSRCS_DIR)
SRC_CC += $(GENSRCS_OUTPUT)

DUMMY := $(shell mkdir -p $(GENSRCS_DIR))
DUMMY := $(shell $(GENSRCS_TOOL) $(GENSRCS_BASEDIR)/$(GENSRCS_SUBDIR) $(GENSRCS_HEADERS) > $(GENSRCS_DIR)/$(GENSRCS_OUTPUT).tmp \
            && diff -q $(GENSRCS_DIR)/$(GENSRCS_OUTPUT).tmp $(GENSRCS_DIR)/$(GENSRCS_OUTPUT) 2> /dev/null \
                || mv $(GENSRCS_DIR)/$(GENSRCS_OUTPUT).tmp $(GENSRCS_DIR)/$(GENSRCS_OUTPUT) 2>/dev/null)
