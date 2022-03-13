$source = "hello_world_add_function"

emcc --no-entry -O0 "$($source).cpp" -o "$($source).js" -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall,addFunction -s ALLOW_TABLE_GROWTH

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"