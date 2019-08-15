ifneq ($(ANDROID_NAME),)

check_android_vars := $(call _assert,$(ANDROID_PORT), ANDROID_PORT not set for $(ANDROID_NAME))
check_android_vars := $(call _assert,$(ANDROID_SECTIONS), ANDROID_SECTIONS not set for $(ANDROID_NAME))
check_android_vars := $(call _assert,$(ANDROID_BUILDTYPE), ANDROID_BUILDTYPE not set for $(ANDROID_NAME))

ANDROID_OUTPUT := $(LIB_CACHE_DIR)/$(ANDROID_BUILDTYPE)/$(ANDROID_NAME)/android.mk

ANDROID_INCLUDES   ?= . include
ANDROID_SOURCES    ?= .
ANDROID_BUILDFILES ?= Android.bp

ANDROID_PORTDIR     = $(call select_from_ports,gart_$(ANDROID_PORT))
check_android_vars := $(call _assert,$(ANDROID_PORTDIR), Port $(ANDROID_PORT) not found for $(ANDROID_NAME))

ANDROID_BASEDIR		= $(ANDROID_PORTDIR)/$(ANDROID_DIR)

# Set Android-compatible architecture
ifeq ($(filter-out $(SPECS),x86_32),)
ANDROID_ARCH = x86
else ifeq ($(filter-out $(SPECS),x86_64),)
ANDROID_ARCH = x86_64
else ifeq ($(filter-out $(SPECS),arm_v7a),)
ANDROID_ARCH = arm
else ifeq ($(filter-out $(SPECS),arm_v8a),)
ANDROID_ARCH = arm64
else ifeq ($(filter-out $(SPECS),linux),)
# Assume x86_64 for base-linux
ANDROID_ARCH = x86_64
else ifeq ($(filter-out $(SPECS),arm_v6),)
$(error Unsupported architecture: arm_v6)
else ifeq ($(filter-out $(SPECS),riscv),)
$(error Unsupported architecture: riscv)
else
$(error Configuration error (could not determine architecture))
endif

COMMAND := \
	mkdir -p $(dir $(ANDROID_OUTPUT)) && \
	$(call select_from_repositories,tool/gnoos) \
		--basedir $(ANDROID_BASEDIR) \
		--prefix $(ANDROID_NAME)_ \
		--output $(ANDROID_OUTPUT) \
		$(addprefix --input ,$(ANDROID_BUILDFILES)) \
		$(ANDROID_SECTIONS) && \
	echo "$(ANDROID_NAME)_BASEDIR = $(ANDROID_BASEDIR)" >> $(ANDROID_OUTPUT) && \
	echo "$(ANDROID_NAME)_EXPORT_INCLUDE_DIRS += $(ANDROID_INCLUDES)" >> $(ANDROID_OUTPUT)

DUMMY := $(shell $(COMMAND))

include $(ANDROID_OUTPUT)

SRC_C   += $(filter-out $(ANDROID_EXCLUDE_C),$(filter %.c,$($(ANDROID_NAME)_SRCS)))
SRC_CC  += $(filter-out $(ANDROID_EXCLUDE_CC),$(filter %.cpp %.cc,$($(ANDROID_NAME)_SRCS)))
SRC_S   += $(filter-out $(ANDROID_EXCLUDE_S),$(filter %.S %.s,$($(ANDROID_NAME)_SRCS)))
CC_OPT  += -Werror -D__GENODE__ -D__ANDROID__ $(filter-out $(ANDROID_EXCLUDE_OPT),$($(ANDROID_NAME)_CFLAGS))
CC_CXX_WARN_STRICT =

INCLUDE_DIRS = \
	$($(ANDROID_NAME)_EXPORT_INCLUDE_DIRS) \
	$($(ANDROID_NAME)_LOCAL_INCLUDE_DIRS) \
	$($(ANDROID_NAME)_INCLUDE_DIRS)

INC_DIR := $(REP_DIR)/include/gart $(INC_DIR) $(addprefix $(ANDROID_BASEDIR)/,$(sort $(INCLUDE_DIRS)))

vpath % $(addprefix $(ANDROID_BASEDIR)/,$(ANDROID_SOURCES))

# Include these libraries by default
LIBS += libc cxx stdcxx
LIBS += $(filter-out $(ANDROID_EXCLUDE_LIBS),$($(ANDROID_NAME)_SHARED_LIBS) $($(ANDROID_NAME)_STATIC_LIBS))

else
$(error ANDROID_NAME not set)
endif
