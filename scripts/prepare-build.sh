#!/bin/sh

cd "$(dirname "$0")"/..

if [ -e libdvbtee ]; then
    echo libdvbtee sources present
else
    git clone git://github.com/mkrufky/libdvbtee.git
fi

cd libdvbtee

if [ -e libdvbpsi/bootstrap ]; then
    echo libdvbpsi sources present
else
    rm -rf libdvbpsi
    git clone git://github.com/mkrufky/libdvbpsi.git
    cd libdvbpsi
    touch .dont_del
    cd ..
fi
