#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_BOOSTORG_JSON="TRUE"
readonly INSTALL_BOOSTORG_ALL="TRUE"
readonly INSTALL_GTEST="TRUE"
readonly INSTALL_PUGIXML="TRUE"

cd $(dirname $0)

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
	# Dependencies for using boostorg json headeronly follow
	git clone https://github.com/boostorg/config.git
	git clone https://github.com/boostorg/assert.git
	git clone https://github.com/boostorg/throw_exception.git
	git clone https://github.com/boostorg/core.git
	git clone https://github.com/boostorg/container.git
	git clone https://github.com/boostorg/move.git
	git clone https://github.com/boostorg/static_assert.git
	git clone https://github.com/boostorg/intrusive.git
	git clone https://github.com/boostorg/system.git
	git clone https://github.com/boostorg/mp11.git
	git clone https://github.com/boostorg/align.git
fi
if [ "TRUE" == "${INSTALL_BOOSTORG_ALL}" ] ; then
    sudo apt-get update
    sudo apt-get install libboost-all-dev
fi
popd

popd

pushd ./deps_install_workarea
if [ "TRUE" == "${INSTALL_GTEST}" ] ; then
	../install_gtest.sh
fi
popd

pushd ../
if [ ! -d external/pugixml ] ; then
	mkdir external/pugixml
fi

pushd external/pugixml
if [ "TRUE" == "${INSTALL_PUGIXML}" ] ; then
	wget https://github.com/zeux/pugixml/releases/download/v1.12/pugixml-1.12.tar.gz
	tar -xzvf pugixml-1.12.tar.gz
	if [ ! -d src ] ; then
		mkdir src
	fi
	if [ ! -d include ] ; then
		mkdir include
	fi
	mv pugixml-1.12/src/pugixml.cpp ./src/
	mv pugixml-1.12/src/pugiconfig.hpp ./include/
	mv pugixml-1.12/src/pugixml.hpp ./include/
fi
popd

popd

rm -r ./deps_install_workarea
