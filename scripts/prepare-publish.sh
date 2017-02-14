#!/bin/sh

cd "$(dirname "$0")"/..

rm -rf _stage
mkdir -p _stage
cd _stage
git clone .. node-dvbtee
cd node-dvbtee

sed -i s/"scripts\/prepare-build.sh; scripts\/configure-build.sh"/"scripts\/configure-build.sh"/1 package.json
sed -i s/"^libdvbtee"/""/1 .gitignore

scripts/prepare-build.sh

npm publish

cd ../..
