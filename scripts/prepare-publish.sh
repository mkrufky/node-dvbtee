#!/bin/sh

cd "$(dirname "$0")"/..

rm -rf _stage
mkdir -p _stage
cd _stage
git clone .. node-dvbtee
cd node-dvbtee

sed -i s/"^libdvbtee"/""/1 .gitignore

npm publish

cd ../..
