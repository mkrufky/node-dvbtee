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
else
    rm -rf libdvbpsi
    git clone git://github.com/mkrufky/libdvbpsi.git
    cd libdvbpsi
    touch .dont_del
    cd ..
fi

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
