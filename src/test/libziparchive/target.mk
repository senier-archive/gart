TARGET = libziparchive_test

ANDROID_PORT	  = core
ANDROID_DIR		  = system/core/libziparchive
ANDROID_SECTIONS = /cc_test[@name=ziparchive-tests]
ANDROID_EXCLUDE_OPT = -Wold-style-cast

include $(call select_from_repositories,lib/mk/android-prg.inc)

LIBS += gtest gart_main

TESTDATA = bad_crc.zip declaredlength.zip dummy-update.zip large.zip valid.zip

$(TARGET): $(INSTALL_DIR)/testdata.tar

$(INSTALL_DIR)/testdata.tar:
	$(VERBOSE)tar cf $@ -C $(ANDROID_BASEDIR)/testdata/ $(TESTDATA)

clean:
	$(VERBOSE)rm testdata.tar
