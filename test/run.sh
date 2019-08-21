#!/bin/sh -eu

create_builddir()
{

ARCH=$1
GD=$2

echo "Setting up /genode_build/${ARCH} for ${ARCH}, source dir ${GD}"

# Configure build
mkdir -p /genode_build/${ARCH}/etc
ln -sf ${GD}/tool/builddir/build.mk /genode_build/${ARCH}/Makefile
echo "SPECS += ${ARCH}" > /genode_build/${ARCH}/etc/specs.conf

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

# Install pyparsing
apt update
apt -y install python3-pyparsing xorriso qemu-system

# Test gnoos
cd /gart/tool
./gnoos --test

# Update Genode repository and check out sculpt-19.07
cd ${GENODE_DIR}
git fetch --all
git checkout componolit/master

# Setup external repos
git clone -b gtest_base_linux https://github.com/Componolit/genode-world.git ${GENODE_DIR}/repos/world
ln -sf /gart ${GENODE_DIR}/repos/gart

# Set up build dir
create_builddir x86_64 ${GENODE_DIR}
create_builddir arm_v8a ${GENODE_DIR}
create_builddir arm_v7a ${GENODE_DIR}

# Prepare ports
/genode/tool/ports/prepare_port googletest gart_core

# Tests to run
TESTS="
   run/log
   run/gtest
   run/test/libbase
   run/test/liblog
   run/test/libmetricslogger
"


make -C /genode_build/x86_64 ${TESTS} KERNEL=linux BOARD=linux EXT_RUN_OPT="--include power_on/linux --include log/linux"
make -C /genode_build/x86_64 ${TESTS} KERNEL=nova BOARD=pc EXT_RUN_OPT="--include power_on/qemu --include log/qemu --include image/iso"
make -C /genode_build/arm_v8a ${TESTS} KERNEL=foc BOARD=rpi3 EXT_RUN_OPT="--include power_on/qemu --include log/qemu"
make -C /genode_build/arm_v7a ${TESTS} KERNEL=foc BOARD=pbxa9 EXT_RUN_OPT="--include power_on/qemu --include log/qemu"
