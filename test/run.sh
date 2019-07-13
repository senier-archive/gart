#!/bin/sh -eu

apt update
apt -y install python3-pip
pip3 install pyparsing

cd /gart/tool
./gnoos --test
