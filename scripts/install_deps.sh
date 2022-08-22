#!/bin/bash

set -ex

if [ ! -d deps_install_workarea ] ; then
	mkdir deps_install_workarea
fi

pushd deps_install_workarea

sudo apt install libx11-dev libxext-dev

wget https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.24.0.zip
unzip release-2.24.0.zip
pushd SDL-release-2.24.0
./configure
make
sudo make install
popd

wget https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.2.zip
unzip release-2.6.2.zip
pushd SDL_image-release-2.6.2
./configure
make
sudo make install
popd

popd
