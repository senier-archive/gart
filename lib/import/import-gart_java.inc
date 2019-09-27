check_java_vars := $(call _assert,$(JAVA_JAR_NAME), JAVA_JAR_NAME not set for $(LIB))
JAVA_CLASSPATH += $(BUILD_BASE_DIR)/bin/$(JAVA_JAR_NAME).jar
