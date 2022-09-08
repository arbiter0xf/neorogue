#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_BOOSTORG_JSON="TRUE"
readonly INSTALL_BOOSTORG_CONFIG="TRUE"
readonly INSTALL_BOOSTORG_ASSERT="TRUE"
readonly INSTALL_BOOSTORG_THROW_EXCEPTION="TRUE"
readonly INSTALL_BOOSTORG_CORE="TRUE"
readonly INSTALL_BOOSTORG_CONTAINER="TRUE"
readonly INSTALL_BOOSTORG_MOVE="TRUE"
readonly INSTALL_BOOSTORG_STATIC_ASSERT="TRUE"
readonly INSTALL_BOOSTORG_INTRUSIVE="TRUE"
readonly INSTALL_BOOSTORG_SYSTEM="TRUE"
readonly INSTALL_BOOSTORG_MP11="TRUE"
readonly INSTALL_BOOSTORG_ALIGN="TRUE"

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

pushd external/boostorg
if [ "TRUE" == "$INSTALL_BOOSTORG_JSON" ] ; then
	git clone https://github.com/boostorg/json.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_CONFIG" ] ; then
	git clone https://github.com/boostorg/config.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_ASSERT" ] ; then
	git clone https://github.com/boostorg/assert.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_THROW_EXCEPTION" ] ; then
	git clone https://github.com/boostorg/throw_exception.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_CORE" ] ; then
	git clone https://github.com/boostorg/core.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_CONTAINER" ] ; then
	git clone https://github.com/boostorg/container.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_MOVE" ] ; then
	git clone https://github.com/boostorg/move.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_STATIC_ASSERT" ] ; then
	git clone https://github.com/boostorg/static_assert.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_INTRUSIVE" ] ; then
	git clone https://github.com/boostorg/intrusive.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_SYSTEM" ] ; then
	git clone https://github.com/boostorg/system.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_MP11" ] ; then
	git clone https://github.com/boostorg/mp11.git
fi

if [ "TRUE" == "$INSTALL_BOOSTORG_ALIGN" ] ; then
	git clone https://github.com/boostorg/align.git
fi
popd

popd

rmdir deps_install_workarea
