#!/bin/sh

cd "$(dirname "$0")"/../libdvbtee/libdvbpsi

if [ -e config.h ]; then
    echo libdvbpsi configured
else
    ./bootstrap
    ./configure --enable-static --with-pic --disable-shared
fi

cd ..

if [ -e usr/include/dvbpsi/dvbpsi.h ]; then
    echo libdvbpsi headers placed
else
    mkdir -p usr/include/dvbpsi
    cd usr/include/dvbpsi
    ln -sf ../../../libdvbpsi/src/*.h .
    ln -sf ../../../libdvbpsi/src/tables/*.h .
    ln -sf ../../../libdvbpsi/src/descriptors/*.h .
    cd ../../..
fi
