#!/bin/sh -eu

CI_ARCH=$1; shift
CI_KERNEL=$1; shift
CI_BOARD=$1; shift
CI_OPTS=$1; shift
CI_TESTS=$*

env

echo "==========================================="
echo " KERNEL: ${CI_KERNEL}"
echo " BOARD:  ${CI_BOARD}"
echo " ARCH:   ${CI_ARCH}"
echo " OPTS:   ${CI_OPTS}"
echo " TESTS:  ${CI_TESTS}"
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

# Update Genode repository and check out 19.08
cd ${GENODE_DIR}
git fetch --all
git checkout 19.08

# Setup external repos
git clone -b gtest_base_linux https://github.com/Componolit/genode-world.git ${GENODE_DIR}/repos/world
ln -sf /gart ${GENODE_DIR}/repos/gart

# Set up build dir
create_builddir ${CI_ARCH} ${GENODE_DIR}

# Prepare ports
/genode/tool/ports/prepare_port googletest

if [ x${CI_KERNEL} = "xnova" ];
then
   /genode/tool/ports/prepare_port nova
fi

make -C /genode_build/${CI_ARCH} ${CI_TESTS} KERNEL=${CI_KERNEL} BOARD=${CI_BOARD} EXT_RUN_OPT="${CI_OPTS}"
