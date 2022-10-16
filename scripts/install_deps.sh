#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_BOOSTORG_JSON="FALSE"
readonly INSTALL_BOOSTORG_ALL="TRUE"
readonly INSTALL_BOOSTORG_ASIO="FALSE"
readonly INSTALL_GTEST="TRUE"
readonly INSTALL_PUGIXML="TRUE"

cd $(dirname $0)

if [ ! -d deps_install_workarea ] ; then
	mkdir deps_install_workarea
fi

pushd deps_install_workarea

sudo apt update
sudo apt install cmake

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

popd # deps_install_workarea

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
	git clone https://github.com/boostorg/variant2.git
fi

if [ "TRUE" == "${INSTALL_BOOSTORG_ASIO}" ] ; then
	test -d asio || git clone https://github.com/boostorg/asio.git
	test -d date_time || git clone https://github.com/boostorg/date_time.git
	test -d smart_ptr || git clone https://github.com/boostorg/smart_ptr.git
	test -d utility || git clone https://github.com/boostorg/utility.git
	test -d type_traits || git clone https://github.com/boostorg/type_traits.git
	test -d numeric_conversion || git clone https://github.com/boostorg/numeric_conversion.git
	test -d mpl || git clone https://github.com/boostorg/mpl.git
	test -d preprocessor || git clone https://github.com/boostorg/preprocessor.git
	test -d bind || git clone https://github.com/boostorg/bind.git
	test -d regex || git clone https://github.com/boostorg/regex.git
	test -d predef || git clone https://github.com/boostorg/predef.git
fi
popd # external/boostorg
popd # ../

pushd  ./deps_install_workarea
if [ "TRUE" == ${INSTALL_BOOSTORG_ALL} ] ; then
	boost_name="boost_1_80_0"
	boost_tar="${boost_name}.tar.gz"
	wget https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/${boost_tar}
	tar -xvzf ${boost_tar}
	pushd ${boost_name}
	# ./bootstrap.sh --with-libraries='asio json'
	# asio currently not detected as a library. See libraries
	# with --show-libraries
	./bootstrap.sh
	sudo ./b2 install
	popd # ${boost_name}
	rm ${boost_tar}
fi
popd # ./deps_install_workarea

pushd ./deps_install_workarea
if [ "TRUE" == "${INSTALL_GTEST}" ] ; then
	../install_gtest.sh
fi
popd # ./deps_install_workarea

pushd ../
if [ ! -d external/pugixml ] ; then
	mkdir external/pugixml
fi # ../

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
popd # external/pugixml

popd # ../

rm -rf ./deps_install_workarea
