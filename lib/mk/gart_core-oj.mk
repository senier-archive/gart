ANDROID_PORT       = libcore
ANDROID_DIR        = libcore
ANDROID_BUILDFILES = JavaLibrary.bp openjdk_java_files.bp non_openjdk_java_files.bp annotated_java_files.bp
ANDROID_SECTIONS   = /java_library[@name=core-oj]

ANDROID_UNDEFINED += :android_icu4j_src_files :android_icu4j_src_files_for_docs
ANDROID_UNDEFINED += :gen-ojluni-jaif-annotated-srcs

JAVA_SOURCE_DIRS += ojluni/src/main/java
JAVA_SOURCE_DIRS += ojluni/src/lambda/java

include $(call select_from_repositories,lib/mk/android-jar.inc)
