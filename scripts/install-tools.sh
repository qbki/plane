#!/usr/bin/env sh

if [ ! -d "" ]; then
  mkdir tools -p
fi

if [ ! -d "./tools/emsdk" ]; then
  cd tools
  git clone --depth 1 https://github.com/emscripten-core/emsdk.git
  cd emsdk
  ./emsdk install latest
  ./emsdk activate latest
  cd ../..
fi

if [ ! -d "./tools/vcpkg" ]; then
  cd tools
  git clone --depth 1 https://github.com/microsoft/vcpkg.git
  cd vcpkg
  ./bootstrap-vcpkg.sh
  cd ../..
fi
