ANDROID_PORT		= core
ANDROID_DIR			= system/core/libcutils
ANDROID_SECTIONS	= \
	/cc_library[@name=libcutils] \
	/cc_library[@name=libcutils]/target/android \
	/cc_library[@name=libcutils]/target/android_$(call android_arch)

# Mocked interfaces
ANDROID_EXCLUDE_CC += ashmem-dev.cpp
SRC_CC += ashmem-genode.cpp

ANDROID_EXCLUDE_CC += iosched_policy.cpp
SRC_CC += iosched_policy-genode.cpp

ANDROID_EXCLUDE_CC += sched_policy.cpp
SRC_CC += sched_policy-genode.cpp

ANDROID_EXCLUDE_CC += uevent.cpp
SRC_CC += uevent_genode.cpp

# Genode-specific socket support
ANDROID_EXCLUDE_CC += sockets_unix.cpp
SRC_CC += sockets_genode.cpp

# The provided implementation uses IPv6 which is unsupported by Genode. Use IPv4 version.
ANDROID_EXCLUDE_CC += socket_inaddr_any_server_unix.cpp
SRC_CC += socket_inaddr_any_server_genode.cpp

# Dummy tracing interface
ANDROID_EXCLUDE_CC += trace-dev.cpp
SRC_CC += trace-genode.cc

include $(call select_from_repositories,lib/mk/android-lib.inc)
VPATH += $(REP_DIR)/src/libcutils

CC_OPT_multiuser = -Wno-type-limits
CC_OPT_str_parms = -Wno-missing-field-initializers

# Pretend we're building for apple, as this removes fs_mkdirs from the source
# whieh uses mkdirat() which is unavailable on Genode (and Apple ;)
CC_OPT_fs		  = -D__APPLE__
