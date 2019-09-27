ANDROID_PORT       = libcore
ANDROID_DIR        = libcore
ANDROID_BUILDFILES = JavaLibrary.bp openjdk_java_files.bp non_openjdk_java_files.bp annotated_java_files.bp
ANDROID_SECTIONS   = /java_library[@name=core-libart]

ANDROID_UNDEFINED += :gen-ojluni-jaif-annotated-srcs

JAVA_SOURCE_DIRS += dalvik/src/main/java
JAVA_SOURCE_DIRS += json/src/main/java
JAVA_SOURCE_DIRS += libart/src/main/java
JAVA_SOURCE_DIRS += xml/src/main/java
JAVA_SOURCE_DIRS += luni/src/main/java

include $(call select_from_repositories,lib/mk/icu4j.inc)
include $(call select_from_repositories,lib/mk/android-jar.inc)
