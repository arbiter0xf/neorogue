#!/bin/bash

set -ex

cd /tmp

echo "================"
echo "Installing Meson"
echo "================"
sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build
pip3 install --user meson
