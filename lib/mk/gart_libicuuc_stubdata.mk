ANDROID_PORT		= icu
ANDROID_DIR			= icu/icu4c/source
ANDROID_SECTIONS	= /cc_library_static[@name=libicuuc_stubdata]

include $(call select_from_repositories,lib/mk/android-lib.inc)

SHARED_LIB =

all: $(BUILD_BASE_DIR)/bin/icu.tar

$(BUILD_BASE_DIR)/bin/icu.tar: $(ANDROID_BASEDIR)/stubdata/icudt*l.dat
	$(VERBOSE)tar -cf $@ -C $(ANDROID_BASEDIR)/stubdata $(notdir $<)
