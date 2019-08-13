#!/bin/sh -eu

GENODE_DIR=/genode

# Install pyparsing
apt update
apt -y install python3-pyparsing

# Test gnoos
cd /gart/tool
./gnoos --test

# Update Genode repository and check out sculpt-19.07
cd ${GENODE_DIR}
git fetch --all
git checkout sculpt-19.07

# Setup external repos
git clone https://github.com/genodelabs/genode-world.git ${GENODE_DIR}/repos/world
ln -sf /gart ${GENODE_DIR}/repos/gart

# Configure build
mkdir -p /genode_build/etc
ln -sf ${GENODE_DIR}/tool/builddir/build.mk /genode_build/Makefile
echo 'SPECS += x86_64' > /genode_build/etc/specs.conf

cat << EOF > /genode_build/etc/build.conf
GENODE_DIR := ${GENODE_DIR}
BASE_DIR := ${GENODE_DIR}/repos/base
CONTRIB_DIR := ${GENODE_DIR}/contrib

MAKE += -j8
KERNEL ?= linux
BOARD ?= linux

RUN_OPT += --include power_on/linux --include log/linux --include boot_dir/linux

REPOSITORIES += ${GENODE_DIR}/repos/base-linux
REPOSITORIES += ${GENODE_DIR}/repos/base
REPOSITORIES += ${GENODE_DIR}/repos/os
REPOSITORIES += ${GENODE_DIR}/repos/libports
REPOSITORIES += ${GENODE_DIR}/repos/ports
REPOSITORIES += ${GENODE_DIR}/repos/world
REPOSITORIES += ${GENODE_DIR}/repos/gart
EOF

# Prepare ports
/genode/tool/ports/prepare_port googletest

# Tests to run
COMMON_TESTS = run/log
LINUX_TESTS =
NOVA_TESTS = run/gtest

make -C /genode_build KERNEL=linux BOARD=linux $(COMMON_TESTS) $(LINUX_TESTS)
make -C /genode_build KERNEL=nova BOARD=pc $(COMMON_TESTS) $(NOVA_TESTS)
