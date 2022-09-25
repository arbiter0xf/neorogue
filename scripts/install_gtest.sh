#!/bin/bash

set -ex

if [ ! -d libs ] ; then
	mkdir libs
fi

if [ ! -d /opt/gtest/include ] ; then
	sudo mkdir -p /opt/gtest/include
fi

if [ ! -d /opt/gtest/lib ] ; then
	sudo mkdir -p /opt/gtest/lib
fi

cd libs

gtest_file="release-1.12.1.tar.gz"
extracted_file="googletest-release-1.12.1"
wget https://github.com/google/googletest/archive/${gtest_file}
tar -xvf ${gtest_file}

cd ${extracted_file}
mkdir build
cd build
cmake ../
make

sudo cp -a ../googletest/include/gtest /opt/gtest/include
sudo cp -a lib/libgtest.a /opt/gtest/lib
sudo cp -a lib/libgtest_main.a /opt/gtest/lib
