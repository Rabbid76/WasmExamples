#!/bin/sh

source="dynamic_linking"
emscripten_folder="/Users/$USER/source/emscripten/emsdk/upstream/emscripten"


$emscripten_folder/emcc \
    --no-entry \
    --bind \
    -O0 \
    hello.cpp \
    hello_world.cpp \
    $source.cpp \
    -o $source.js \
    -s WASM=1

echo "http://localhost:8090/$source.html"
python3 -m http.server 8090 --directory "./"