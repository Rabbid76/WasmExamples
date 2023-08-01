#!/bin/sh

source="dynamic_linking"
emscripten_folder="/Users/$USER/source/emscripten/emsdk/upstream/emscripten"

$emscripten_folder/emcc \
    hello.cpp \
    -o hello.wasm \
    -s SIDE_MODULE=1

$emscripten_folder/emcc \
    hello_world.cpp \
    -o hello_world.wasm \
    -s SIDE_MODULE=1  

$emscripten_folder/emcc \
    --no-entry \
    --bind \
    -O0 \
    hello.wasm \
    hello_world.wasm \
    $source.cpp \
    -o $source.js \
    -s WASM=1 \
    -s MAIN_MODULE=1


echo "http://localhost:8090/$source.html"
python3 -m http.server 8090 --directory "./"