GART_GTEST_NAME = Transaction
SRC_JAVA = InstanceFieldsTest.java StaticArrayFieldsTest.java StaticFieldsTest.java Transaction.java

include $(call select_from_repositories,lib/mk/gart_gtest.inc)
