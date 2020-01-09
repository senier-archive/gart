check_vars := $(call _assert,$(GART_GTEST_NAME), GART_GTEST_NAME not set)

SRC_JAVA ?= $(GART_GTEST_NAME).java
ART_DIR = $(call _port_dir,gart_art)/art

JAVA_JAR_NAME = art-gtest-$(GART_GTEST_NAME)
JAVA_BASE_DIR = $(ART_DIR)/test/$(GART_GTEST_NAME)

TARGET = $(JAVA_JAR_NAME).jar

include $(call select_from_repositories,lib/mk/gart_java.inc)
