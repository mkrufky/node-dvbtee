#!/bin/sh

cd "$(dirname "$0")"/..

if [ -e libdvbtee ]; then
    echo libdvbtee already present
else
    git clone git://github.com/mkrufky/libdvbtee.git
fi

cd libdvbtee

if [ -e libdvbpsi/bootstrap ]; then
    echo libdvbpsi already present
    cd libdvbpsi
else
    rm -rf libdvbpsi
    git clone git://github.com/mkrufky/libdvbpsi.git
    cd libdvbpsi
    touch .dont_del
fi

if [ -e config.h ]; then
    echo libdvbpsi already configured
else
    ./bootstrap
    ./configure --enable-static --with-pic --disable-shared
fi

cd ..

if [ -e usr/include/dvbpsi/dvbpsi.h ]; then
    echo libdvbpsi headers already placed
else
    mkdir -p usr/include/dvbpsi
    cd usr/include/dvbpsi
    ln -sf ../../../libdvbpsi/src/*.h .
    ln -sf ../../../libdvbpsi/src/tables/*.h .
    ln -sf ../../../libdvbpsi/src/descriptors/*.h .
    cd ../../..
fi

mkdir -p m4
if [ -e aclocal.m4 ]; then
    echo "configuration installed..."
else
    autoreconf --install
fi

if [ -e config.h ]; then
    echo libdvbtee already configured
else
    ./configure --enable-static --with-pic --disable-shared --enable-dvbpsibuild --disable-examples
fi
