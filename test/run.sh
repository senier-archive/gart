#!/bin/sh -eu

CI_ARCH=$1
CI_KERNEL=$2
CI_BOARD=$3
CI_OPTS=$4

# Tests to run
TESTS="
   run/log
   run/gtest
   run/test/libbase
   run/test/liblog
   run/test/libmetricslogger
   run/test/libz
   run/test/libcutils
   run/test/libutils
   run/test/jni_invocation
   run/test/libnativebridge/CodeCacheCreate
   run/test/libnativebridge/CodeCacheExists
   run/test/libnativebridge/CodeCacheStatFail
   run/test/libnativebridge/CompleteFlow
   run/test/libnativebridge/InvalidCharsNativeBridge
   run/test/libnativebridge/NativeBridge3CreateNamespace
   run/test/libnativebridge/NativeBridge3GetError
   run/test/libnativebridge/NativeBridge3InitAnonymousNamespace
   run/test/libnativebridge/NativeBridge3IsPathSupported
   run/test/libnativebridge/NativeBridge3LoadLibraryExt
   run/test/libnativebridge/NativeBridge3UnloadLibrary
   run/test/libnativebridge/NativeBridgeVersion
   run/test/libnativebridge/NeedsNativeBridge
   run/test/libnativebridge/PreInitializeNativeBridgeFail1
   run/test/libnativebridge/PreInitializeNativeBridgeFail2
   run/test/libnativebridge/PreInitializeNativeBridge
   run/test/libnativebridge/ReSetupNativeBridge
   run/test/libnativebridge/UnavailableNativeBridge
   run/test/libnativebridge/ValidNameNativeBridge
"

if [ ${CI_ARCH} != "arm_v7a" ];
then
   TESTS="${TESTS} run/test/liblz4"
fi

env

echo "==========================================="
echo " KERNEL: ${CI_KERNEL}"
echo " BOARD:  ${CI_BOARD}"
echo " ARCH:   ${CI_ARCH}"
echo " OPTS:   ${CI_OPTS}"
echo " TESTS:  ${TESTS}"
echo "==========================================="

create_builddir()
{

ARCH=$1
GD=$2

echo "Setting up /genode_build/${ARCH} for ${ARCH}, source dir ${GD}"

# Configure build
mkdir -p /genode_build/${ARCH}/etc
ln -sf ${GD}/tool/builddir/build.mk /genode_build/${ARCH}/Makefile
echo "SPECS += ${ARCH}" > /genode_build/${ARCH}/etc/specs.conf

cat << EOF > /genode_build/${ARCH}/etc/tools.conf
CUSTOM_CC  = ccache \$(CROSS_DEV_PREFIX)gcc
CUSTOM_CXX = ccache \$(CROSS_DEV_PREFIX)g++
EOF

cat << EOF > /genode_build/${ARCH}/etc/build.conf
GENODE_DIR := ${GD}
BASE_DIR := ${GD}/repos/base
CONTRIB_DIR := ${GD}/contrib

MAKE += -j8

RUN_OPT += \$(EXT_RUN_OPT) --include boot_dir/\$(KERNEL)
REPOSITORIES += ${GD}/repos/base-\$(KERNEL)
REPOSITORIES += ${GD}/repos/base
REPOSITORIES += ${GD}/repos/os
REPOSITORIES += ${GD}/repos/libports
REPOSITORIES += ${GD}/repos/ports
REPOSITORIES += ${GD}/repos/world
REPOSITORIES += ${GD}/repos/gart
EOF
}

GENODE_DIR=/genode
export CCACHE_DIR=/ccache

# Install pyparsing
apt update
apt -y install python3-pyparsing xorriso qemu-system ccache

# Test gnoos
cd /gart/tool
./gnoos --test

# Update Genode repository and check out componolit/gart branch
cd ${GENODE_DIR}
git fetch --all
git checkout componolit/gart

# Setup external repos
git clone -b gtest_base_linux https://github.com/Componolit/genode-world.git ${GENODE_DIR}/repos/world
ln -sf /gart ${GENODE_DIR}/repos/gart

# Set up build dir
create_builddir ${CI_ARCH} ${GENODE_DIR}

# Prepare ports
/genode/tool/ports/prepare_port googletest gart_core gart_libz gart_libnativehelper gart_liblz4

if [ x${CI_KERNEL} = "xnova" ];
then
   /genode/tool/ports/prepare_port nova
fi

make -C /genode_build/${CI_ARCH} ${TESTS} KERNEL=${CI_KERNEL} BOARD=${CI_BOARD} EXT_RUN_OPT="${CI_OPTS}"
