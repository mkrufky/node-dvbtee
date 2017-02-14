#!/bin/sh

cd "$(dirname "$0")"/..

rm -rf _stage
mkdir -p _stage
cd _stage
git clone .. node-dvbtee
cd node-dvbtee

scripts/prepare-build.sh

sed -i s/"scripts\/prepare-build.sh; scripts\/configure-build.sh"/"scripts\/configure-build.sh"/1 package.json
sed -i s/"^libdvbtee"/""/1 .gitignore

cd libdvbtee
sed -i s/"^libdvbpsi\/"/""/1 .gitignore
cd ..

npm publish

cd ../..
