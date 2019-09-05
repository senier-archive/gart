ANDROID_NAME = $(LIB)

# Include import file of the current library to make definitions available
-include $(REP_DIR)/lib/import/import-$(ANDROID_NAME).mk

ANDROID_BUILDTYPE = lib
include $(call select_from_repositories,lib/mk/android.inc)

ifeq ($(ANDROID_STATIC_LIB),)
SHARED_LIB = yes
endif
