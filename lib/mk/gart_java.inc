check_java_vars := $(call _assert,$(JAVA_BASE_DIR), JAVA_BASE_DIR not set for $(LIB))
check_java_vars := $(call _assert,$(JAVA_JAR_NAME), JAVA_JAR_NAME not set for $(LIB))
check_java_vars := $(call _assert,$(SRC_JAVA), SRC_JAVA not set for $(LIB))

GART_JDK9_PATH = $(call select_from_ports,gart_jdk9)/jdk9/linux-x86/bin

# `dx` requires `java` to be in PATH
export PATH := $(GART_JDK9_PATH):$(PATH)

MSG_JAVAC ?= @$(ECHO) "    JAVAC   "
MSG_D8    ?= @$(ECHO) "       D8   "

JAVAC ?= $(GART_JDK9_PATH)/javac
JAR ?= $(GART_JDK9_PATH)/jar
D8 ?= $(call select_from_ports,gart_r8)/r8/buildtools/d8-master.jar

JAVA_OPTS += \
	-source 1.8 \
	-target 1.8 \
	-Xlint:none \
	-nowarn \
	-Xmaxerrs 9999999 \
	-encoding UTF-8 \
	-g \
	-XDskipDuplicateBridges=true \
	-XDstringConcat=inline \
	-bootclasspath "" \
	-Xpkginfo:always \
	-implicit:none

E :=
SPACE := $E $E

JAR_BASE_NAME = $(BUILD_BASE_DIR)/bin/$(JAVA_JAR_NAME)
FILELIST = $(LIB_CACHE_DIR)/$(LIB)/files.lst
SRCLIST = $(LIB_CACHE_DIR)/$(LIB)/sources.lst

MIN_SDK_VERSION = 26

D8_OPTS += --min-api $(MIN_SDK_VERSION)

DESUGAR_OPTS += --min_sdk_version=$(MIN_SDK_VERSION)
DESUGAR_OPTS += --desugar_try_with_resources_if_needed=false
DESUGAR_OPTS += --allow_empty_bootclasspath

JAVA_BASE_DIRS += $(JAVA_BASE_DIR)

findsrc = $(foreach d,$(sort $2),$(wildcard $d/$1))
JAVA_SOURCES = $(foreach F,$(SRC_JAVA),$(call findsrc,$F,$(sort $(JAVA_BASE_DIRS))))

LIBCORE_INCLUDE_DIRS += ojluni/src/main/java
LIBCORE_INCLUDE_DIRS += ojluni/src/lambda/java
LIBCORE_INCLUDE_DIRS += ojluni/src/main/resources
LIBCORE_INCLUDE_DIRS += luni/src/main/java
LIBCORE_INCLUDE_DIRS += libart/src/main/java
LIBCORE_INCLUDE_DIRS += dalvik/src/main/java
LIBCORE_INCLUDE_DIRS += xml/src/main/java
LIBCORE_INCLUDE_DIRS += json/src/main/java

JAVA_INCLUDE_DIRS  =
JAVA_INCLUDE_DIRS += $(call select_from_ports,gart_icu)/icu/android_icu4j/src/main/java
JAVA_INCLUDE_DIRS += $(addprefix $(call select_from_ports,gart_libcore)/libcore/,$(LIBCORE_INCLUDE_DIRS))
JAVA_CLASS_PATHS   = $(subst $(SPACE),:,$(strip $(JAVA_INCLUDE_DIRS)))

ifneq ($(called_from_lib_mk),)
# FIXME: Can we depend on the .a file?
all: $(JAR_BASE_NAME).jar
endif

$(JAR_BASE_NAME).jar:
	$(MSG_JAVAC)$(notdir $@)
	$(VERBOSE)$(file >$(SRCLIST)) $(foreach O, $(JAVA_SOURCES),$(file >>$(SRCLIST),$O))
	$(VERBOSE)set -o pipefail; $(JAVAC) $(JAVA_OPTS) -d $(LIB_CACHE_DIR)/$(LIB) -classpath $(JAVA_CLASS_PATHS) @$(SRCLIST) |& sed '/^Note:/d'
	$(MSG_D8)$(notdir $@)
	$(VERBOSE)find $(LIB_CACHE_DIR)/$(LIB) -type f -name *.class -printf '%P\n' > $(FILELIST)
	$(VERBOSE)java -jar $(D8) $(D8_OPTS) --output $@ $$(cat $(FILELIST))
	$(VERBOSE)rm -f $(FILELIST) $(SRCLIST)
