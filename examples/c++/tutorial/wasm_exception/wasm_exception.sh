#!/bin/sh

source="wasm_exception"
emscripten_folder="/Users/$USER/source/emscripten/emsdk/upstream/emscripten"

$emscripten_folder/emcc $source.cpp -o $source.html -s WASM=1 -fwasm-exceptions

echo "http://localhost:8090/$source.html"
python3 -m http.server 8090 --directory "./"
