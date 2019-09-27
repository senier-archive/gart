check_java_vars := $(call _assert,$(JAVA_BASE_DIR), JAVA_BASE_DIR not set for $(LIB))
check_java_vars := $(call _assert,$(JAVA_JAR_NAME), JAVA_JAR_NAME not set for $(LIB))
check_java_vars := $(call _assert,$(SRC_JAVA), SRC_JAVA not set for $(LIB))

JAVAC ?= $(call select_from_ports,gart_jdk9)/jdk9/linux-x86/bin/javac
JAR ?= $(call select_from_ports,gart_jdk9)/jdk9/linux-x86/bin/jar

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
	-Xpkginfo:always

E :=
SPACE := $E $E

# Add directories pointing to java.jang to classpath
LIBCORE_BASEDIR = $(call select_from_ports,gart_libcore)/libcore
LIBCORE_SUBDIRS += ojluni/src/main/java
LIBCORE_SUBDIRS += ojluni/src/lambda/java
LIBCORE_SUBDIRS += ojluni/src/main/resources
LIBCORE_SUBDIRS += luni/src/main/java
LIBCORE_SUBDIRS += libart/src/main/java
LIBCORE_SUBDIRS += dalvik/src/main/java
LIBCORE_SUBDIRS += xml/src/main/java
LIBCORE_SUBDIRS += json/src/main/java
JAVA_SOURCEPATH += $(addprefix $(LIBCORE_BASEDIR)/,$(LIBCORE_SUBDIRS))

JAVA_SOURCEPATH +=  $(addprefix $(JAVA_BASE_DIR)/,$(JAVA_SOURCE_DIRS))

# icu4j is always required
JAVA_SOURCEPATH += $(call select_from_ports,gart_icu)/icu/android_icu4j/src/main/java

CP = $(subst $(SPACE),:,$(strip $(JAVA_CLASSPATH)))
SP = $(subst $(SPACE),:,$(strip $(JAVA_SOURCEPATH)))

JAR_NAME = $(BUILD_BASE_DIR)/bin/$(JAVA_JAR_NAME).jar
MANIFEST = $(LIB_CACHE_DIR)/$(LIB)/Manifest
FILELIST = $(LIB_CACHE_DIR)/$(LIB)/files.lst

ifneq ($(called_from_lib_mk),)
# FIXME: Can we depend on the .a file?
all: $(JAR_NAME)
endif

extract = $(filter-out $1,$(foreach d,$2,$(patsubst $d/%,$d,$1)))

$(JAR_NAME): $(addprefix $(LIB_CACHE_DIR)/$(LIB)/,$(SRC_JAVA:.java=.class)) | $(MANIFEST)
	$(MSG_MERGE)$(notdir $@)
	$(file >$(FILELIST)) $(foreach O,$(patsubst $(LIB_CACHE_DIR)/$(LIB)/%,%,$^),$(file >>$(FILELIST),$O))
	$(VERBOSE)$(JAR) cfm $@ $(MANIFEST) -C $(LIB_CACHE_DIR)/$(LIB) @$(FILELIST)
	$(VERBOSE)rm -f $(FILELIST)

$(MANIFEST):
	$(VERBOSE)echo "Class-Path: $(JAVA_SOURCE_DIRS)/" > $@

$(LIB_CACHE_DIR)/$(LIB)/%.class: $(JAVA_BASE_DIR)/%.java
	$(MSG_COMP)$*.java
	$(VERBOSE)set -o pipefail; $(JAVAC) $(JAVA_OPTS) -d $(LIB_CACHE_DIR)/$(LIB)/$(call extract,$*,$(JAVA_SOURCE_DIRS)) -classpath "$(CP)" -sourcepath "$(SP)" $< |& sed '/^Note:/d'
