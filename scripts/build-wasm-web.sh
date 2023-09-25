#!/bin/sh
set -e
echo $1
cd $1
emcmake cmake -S . -B build-wasm-web -G Ninja -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release
cmake --build build-wasm-web

mkdir -p temp

mkdir -p releases

cp -r build-wasm-web/bin temp/build-wasm-web

cd temp

zip -r ../releases/wasm-web.zip ./build-wasm-web/