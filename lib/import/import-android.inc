ifeq ($(ANDROID_LIB),)
$(error ANDROID_LIB not set)
endif

-include $(LIB_CACHE_DIR)/lib/$(ANDROID_LIB)/android.mk

INCLUDE_DIRS = \
	$($(ANDROID_LIB)_EXPORT_INCLUDE_DIRS) \
	$($(ANDROID_LIB)_INCLUDE_DIRS)

INC_DIR := $(INC_DIR) $(addprefix $($(ANDROID_LIB)_BASEDIR)/,$(sort $(INCLUDE_DIRS)))
