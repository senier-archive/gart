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

internal_android_arch = \
   $(if $(filter $(SPECS), x86_32),x86, \
      $(if $(filter $(SPECS), x86_64),x86_64, \
         $(if $(filter $(SPECS), arm_v7a),arm, \
            $(if $(filter $(SPECS), arm_v8a),arm64, \
               $(if $(filter $(SPECS), linux),x86_64, \
                  $(if $(filter $(SPECS), arm_v6), $(error arm_v6 unsupported on Android), \
                     $(if $(filter $(SPECS), riscv), $(error RISCV unsupported on Android), \
                        $(error Unsupported architecture))))))))

android_arch = $(strip $(call internal_android_arch))

# Set Android-compatible architecture
ANDROID_ARCH = $(call android_arch)

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

SRC_C  += $(filter-out $(ANDROID_EXCLUDE_C),$(filter %.c,$($(ANDROID_NAME)_SRCS)))
SRC_CC += $(filter-out $(ANDROID_EXCLUDE_CC),$(filter %.cpp %.cc,$($(ANDROID_NAME)_SRCS)))
SRC_S  += $(filter-out $(ANDROID_EXCLUDE_S),$(filter %.S %.s,$($(ANDROID_NAME)_SRCS)))
CC_OPT += -Werror -D__GENODE__ -D__ANDROID__ $(filter-out $(ANDROID_EXCLUDE_OPT),$($(ANDROID_NAME)_CFLAGS))
CC_OPT += -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter
CC_CXX_WARN_STRICT =

INCLUDE_DIRS = \
	$($(ANDROID_NAME)_EXPORT_INCLUDE_DIRS) \
	$($(ANDROID_NAME)_LOCAL_INCLUDE_DIRS) \
	$($(ANDROID_NAME)_INCLUDE_DIRS)

INC_DIR := $(REP_DIR)/include/gart $(INC_DIR) $(addprefix $(ANDROID_BASEDIR)/,$(sort $(INCLUDE_DIRS)))

vpath % $(addprefix $(ANDROID_BASEDIR)/,$(ANDROID_SOURCES))

# Include these libraries by default
ANDROID_LIBS = \
   $($(ANDROID_NAME)_SHARED_LIBS) \
   $($(ANDROID_NAME)_STATIC_LIBS) \
   $($(ANDROID_NAME)_WHOLE_STATIC_LIBS)

# Filter out debug libraries
NON_DEBUG_LIBS = $(sort $(ANDROID_LIBS:_debug=))

LIBS += libc cxx stdcxx
LIBS += $(addprefix gart_,$(filter-out $(ANDROID_EXCLUDE_LIBS),$(NON_DEBUG_LIBS)))

else
$(error ANDROID_NAME not set)
endif
