ANDROID_PORT		= core
ANDROID_DIR			= system/core/libcutils
ANDROID_SECTIONS	= \
	/cc_library[@name=libcutils] \
	/cc_library[@name=libcutils]/target/android \
	/cc_library[@name=libcutils]/target/android_$(call android_arch)

ANDROID_EXCLUDE_CC = ashmem-dev.cpp iosched_policy.cpp sched_policy.cpp
SRC_CC				+= ashmem-genode.cpp iosched_policy-genode.cpp sched_policy-genode.cpp

include $(call select_from_repositories,lib/mk/android-lib.inc)
VPATH += $(REP_DIR)/src/libcutils
