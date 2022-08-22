#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_BOOSTORG_JSON="TRUE"

if [ ! -d deps_install_workarea ] ; then
	mkdir deps_install_workarea
fi

pushd deps_install_workarea

if [ "TRUE" == "$INSTALL_SDL" ] ; then
	sudo apt install libx11-dev libxext-dev

	wget https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.24.0.zip
	unzip release-2.24.0.zip
	pushd SDL-release-2.24.0
	./configure
	make
	sudo make install
	popd
fi

if [ "TRUE" == "$INSTALL_SDL_IMAGE" ] ; then
	sudo apt install libx11-dev libxext-dev

	wget https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.2.zip
	unzip release-2.6.2.zip
	pushd SDL_image-release-2.6.2
	./configure
	make
	sudo make install
	popd
fi

popd

pushd ../
if [ ! -d external ] ; then
	mkdir external
fi

if [ ! -d external/boostorg ] ; then
	mkdir external/boostorg
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_JSON" ] ; then
	pushd external/boostorg
	git clone https://github.com/boostorg/json.git
	popd
fi

popd
