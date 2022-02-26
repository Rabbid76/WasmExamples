#!/bin/sh

#chmod +x ./hello_world.sh

#source ~/devel/emsdk/emsdk_env.sh 
source_file=hello_world
mkdir bin
mkdir "bin/$source_file"
emcc -s WASM=1 "$source_file.c" -o "./bin/$source_file/$source_file.html"
open "http://localhost:8000/$source_file.html"
python3 -m http.server --directory "./bin/$source_file"