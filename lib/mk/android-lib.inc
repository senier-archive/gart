ANDROID_NAME = $(LIB)
ANDROID_BUILDTYPE = lib
include $(call select_from_repositories,lib/mk/android.inc)
SHARED_LIB = yes

# Include import file of the current library to make definitions available
include $(call select_from_repositories,lib/import/import-gart_$(ANDROID_NAME).mk)
