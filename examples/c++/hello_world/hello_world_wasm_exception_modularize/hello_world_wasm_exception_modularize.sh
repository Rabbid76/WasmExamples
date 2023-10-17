#!/bin/sh

source="hello_world_wasm_exception_modularize"
emscripten_folder="/Users/$USER/source/emscripten/emsdk/upstream/emscripten"

$emscripten_folder/emcc --no-entry --bind -O0 $source.cpp -o $source.js -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="createMyModule" -fwasm-exceptions

echo "http://localhost:8090/$source.html"
python3 -m http.server 8090 --directory "./"
