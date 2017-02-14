#!/bin/sh

cd "$(dirname "$0")"/../libdvbtee/libdvbpsi

if [ -e config.h ]; then
    echo libdvbpsi configured
else
    ./bootstrap
    ./configure --enable-static --with-pic --disable-shared
fi

cd ..

mkdir -p m4
if [ -e aclocal.m4 ]; then
    echo configuration installed
else
    autoreconf --install
fi

if [ -e config.h ]; then
    echo libdvbtee configured
else
    ./configure --enable-static --with-pic --disable-shared --enable-dvbpsibuild --disable-examples
fi
