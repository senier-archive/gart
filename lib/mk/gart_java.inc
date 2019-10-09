check_java_vars := $(call _assert,$(JAVA_BASE_DIR), JAVA_BASE_DIR not set for $(LIB))
check_java_vars := $(call _assert,$(JAVA_JAR_NAME), JAVA_JAR_NAME not set for $(LIB))
check_java_vars := $(call _assert,$(SRC_JAVA), SRC_JAVA not set for $(LIB))

GART_JDK9_PATH = $(call select_from_ports,gart_jdk9)/jdk9/linux-x86/bin

# `dx` requires `java` to be in PATH
export PATH := $(GART_JDK9_PATH):$(PATH)

JAVAC ?= $(GART_JDK9_PATH)/javac
JAR ?= $(GART_JDK9_PATH)/jar
DX ?= $(call select_from_ports,gart_build-tools)/build-tools/common/bin/dx

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
FILELIST = $(LIB_CACHE_DIR)/$(LIB)/files.lst

DEX_OPTS += --min-sdk-version=26

ifneq ($(called_from_lib_mk),)
# FIXME: Can we depend on the .a file?
all: $(JAR_NAME)
endif

VPATH += $(JAVA_SOURCEPATH)

extract = $(filter-out $1,$(foreach d,$2,$(patsubst $d/%,%,$1)))

SRC_JAVA_SORTED = $(sort $(SRC_JAVA))
JAVA_CLASSES = $(call extract,$(SRC_JAVA_SORTED:.java=.class),$(JAVA_SOURCE_DIRS))

ifneq ($(words $(sort $(JAVA_CLASSES))),$(words $(JAVA_CLASSES)))
$(error JAVA_CLASSES has duplicates!)
endif

# Force ordering of java classes to serialize javac invocations
RIGHT = $(filter-out $(firstword $(JAVA_CLASSES)), $(JAVA_CLASSES))
LEFT  = $(filter-out $(lastword $(JAVA_CLASSES)), $(JAVA_CLASSES))
PAIRS = $(join $(LEFT),$(addprefix :,$(RIGHT)))
$(foreach pair,$(PAIRS),$(eval $(pair)))

$(JAR_NAME): $(JAVA_CLASSES)
	$(MSG_MERGE)$(notdir $@)
	$(file >$(FILELIST)) $(foreach O,$(patsubst $(LIB_CACHE_DIR)/$(LIB)/%,%,$^),$(file >>$(FILELIST),$O))
	$(VERBOSE)$(DX) $(DEX_OPTS) --dex --output=$@ --input-list=$(FILELIST)
	$(VERBOSE)rm -f $(FILELIST)

%.class: %.java
	$(MSG_COMP)$*.java
	$(VERBOSE)set -o pipefail; $(JAVAC) $(JAVA_OPTS) -d $(LIB_CACHE_DIR)/$(LIB) -classpath "$(CP)" -sourcepath "$(SP)" $< |& sed '/^Note:/d'
