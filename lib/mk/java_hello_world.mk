TARGET = hello_world

JAVA_JAR_NAME = HelloWorld
JAVA_BASE_DIR = $(REP_DIR)/src/test/java_hello_world
SRC_JAVA = HelloWorld.java

include $(call select_from_repositories,lib/mk/gart_java.inc)

