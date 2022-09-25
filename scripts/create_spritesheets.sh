#!/bin/bash

set -e

readonly spritesheets_dir="spritesheets"

cd $(dirname $0)

pushd ../assets

if [ ! -d "${spritesheets_dir}" ] ; then
	mkdir "${spritesheets_dir}"
fi

if [ -z "$(which TexturePacker)" ] ; then
	echo "Could not find TexturePacker."
	exit 0
fi

readonly directories=$(ls -1 | grep -v -e "${spritesheets_dir}" -e LICENSE.txt -e README.txt -e utility)

for directory in ${directories} ; do
	spritesheet_filename="$(echo ${directory} | sed 's|./||' | sed 's|/|_|g').png"
	spritesheet_coordinates_filename="$(echo ${directory} | sed 's|./||' | sed 's|/|_|g').json"
	spritesheet_filename="${spritesheets_dir}/${spritesheet_filename}"
	spritesheet_coordinates_filename="${spritesheets_dir}/${spritesheet_coordinates_filename}"

	if [ "." == "${directory}" ] ; then
		continue
	fi

	echo Creating spritesheet:
	echo -e "\tdirectory=${directory}"
	echo -e "\tspritesheet_filename=${spritesheet_filename}"
	echo -e "\tspritesheet_coordinates_filename=${spritesheet_coordinates_filename}"

	TexturePacker --sheet ${spritesheet_filename} --format json --data ${spritesheet_coordinates_filename} ${directory}
done

popd
