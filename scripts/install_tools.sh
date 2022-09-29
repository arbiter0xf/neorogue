#!/bin/bash

set -ex

cd /tmp

echo -e "========================"
echo -e "Installing Texturepacker"
echo -e "========================"
wget https://www.codeandweb.com/download/texturepacker/6.0.2/TexturePacker-6.0.2.deb
sudo dpkg -i TexturePacker-6.0.2.deb || true # There may be uninstalled deps
sudo apt-get -f install # Install deps
sudo apt-get install libegl1 # Seems to be missing even after fixing deps above

rm TexturePacker-6.0.2.deb
