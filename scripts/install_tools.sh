#!/bin/bash

set -ex

cd /tmp

echo -e "========================"
echo -e "Installing Texturepacker"
echo -e "========================"
wget https://www.codeandweb.com/download/texturepacker/6.0.2/TexturePacker-6.0.2.deb
sudo dpkg -i TexturePacker-6.0.2.deb || true # There may be uninstalled deps
sudo apt-get -f install # Install deps

# TODO remove when done debugging
echo Find 1
find / -name libEGL.so.1 2> /dev/null || true

sudo apt-get install libegl1 # Seems to be missing even after fixing deps above

# TODO remove when done debugging
echo Find 2
find / -name libEGL.so.1 2> /dev/null || true

rm TexturePacker-6.0.2.deb
