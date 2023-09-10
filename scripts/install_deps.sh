#!/bin/bash

set -ex

# TODO shellcheck
# * Local make target
# * Ran in automation

readonly INSTALL_CHECK_UNIT_TEST_FRAMEWORK="FALSE"
readonly INSTALL_VULKAN="TRUE"
readonly INSTALL_SDL="FALSE"
readonly INSTALL_SDL_IMAGE="FALSE"
readonly INSTALL_BOOSTORG_JSON="FALSE"
readonly INSTALL_BOOSTORG_ALL="FALSE"
readonly INSTALL_BOOSTORG_ASIO="FALSE"
readonly INSTALL_GTEST="FALSE"
readonly INSTALL_PUGIXML="FALSE"

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

if [ "TRUE" == "${INSTALL_CHECK_UNIT_TEST_FRAMEWORK}" ] ; then
	echo "Updating and upgrading packages"
	sudo apt update
	sudo apt -y upgrade

	echo "Installing Check unit test framework"
	sudo apt -y install check
fi

# TODO
# * Write in functions
# * Perform the download in 'deps_install_workarea'
#
# Directories supposedly (other locations also possible) containing Vulkan
# driver manifests:
# 	/etc/vulkan/icd.d/
# 	/usr/share/vulkan/icd.d/
#
# Install based on: https://vulkan.lunarg.com/doc/sdk/1.3.261.1/linux/getting_started.html
if [ "TRUE" == "${INSTALL_VULKAN}" ] ; then
	readonly vulkan_tar_checksum_from_web='d72e6c05c05e4ffc30a11fb52758bd67b04d76601d5c42a5306b58a0099dd9bc'
	readonly vulkansdk_web='vulkansdk-linux-x86_64-1.3.261.1'
	readonly vulkansdk_tar="${vulkansdk_web}.tar.xz"
	readonly vulkansdk_destination="${HOME}/my/tools"
	readonly vulkansdk_local_renamed="${vulkansdk_web}"
	vulkansdk_local=''
	vulkan_tar_checksum_local=''

	if [ ! -d vulkan ] ; then
		mkdir vulkan
	fi

	pushd vulkan

	sudo apt update
	sudo apt -y upgrade
	sudo apt -y install qtbase5-dev libxcb-xinput0 libxcb-xinerama0

	if [ ! -f ./${vulkansdk_tar} ] ; then
		curl -o ${vulkansdk_tar} https://sdk.lunarg.com/sdk/download/1.3.261.1/linux/${vulkansdk_tar}
		vulkan_tar_checksum_local="$(sha256sum ${vulkansdk_tar} | cut -d ' ' -f 1)"

		if [ "${vulkan_tar_checksum_from_web}" != "${vulkan_tar_checksum_local}" ] ; then
			echo "Downloaded vulkan sdk package has unexpected checksum: ${vulkan_tar_checksum_local}"
			exit 1
		fi
	fi

	vulkansdk_local="$(tar --list --file ${vulkansdk_tar} \
		| head -1 \
		| sed -e 's|/$||')"
	tar xf ./${vulkansdk_tar}

	if [ ! -d "${vulkansdk_destination}" ] ; then
		echo ""
		echo "[!] No destination directory for Vulkan SDK [!]"
		echo -n "Please create the directory and set it's path to be "
		echo "value of 'vulkansdk_destination'"
		echo ""
		exit 1
	fi

	mv --verbose "${vulkansdk_local}" "${vulkansdk_local_renamed}"
	mv --verbose \
		"${vulkansdk_local_renamed}" \
		"${vulkansdk_destination}/${vulkansdk_local_renamed}"

	# TODO continue here

	popd # vulkan
fi

popd # ../

sudo rm -rf ./deps_install_workarea
