#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_GTEST="TRUE"
readonly INSTALL_PUGIXML="FALSE"
readonly INSTALL_CMAKE="TRUE"

cd $(dirname $0)

if [ ! -d deps_install_workarea ] ; then
    mkdir deps_install_workarea
fi

pushd deps_install_workarea

if [ "TRUE" == "${INSTALL_CMAKE}" ] ; then
    sudo apt update
    sudo apt install cmake
fi

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

popd # ../

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

sudo rm -rf ./deps_install_workarea
