#!/bin/bash

cd $(dirname $0)

pushd ..

if [ "rogue_forever" != "$(basename $(pwd))" ] ; then
    echo "Unexpected current directory. Please cd to scripts directory and run ./download_assets.sh"
    exit 0
fi

if [ ! -d assets ] ; then
    mkdir assets
fi

pushd assets

wget https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/crawl-tiles/crawl-tiles%20Oct-5-2010.zip
unzip -d ./ 'crawl-tiles Oct-5-2010.zip'
mv 'crawl-tiles Oct-5-2010'/* ./
rm 'crawl-tiles Oct-5-2010.zip'
rmdir 'crawl-tiles Oct-5-2010'

popd
popd
