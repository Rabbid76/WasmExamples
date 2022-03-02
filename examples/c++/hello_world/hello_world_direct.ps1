$source = "hello_world_direct"

emcc --no-entry -O0 "$($source).cpp" -o "$($source).wasm" -s WASM=1 -s EXPORTED_FUNCTIONS=_lerp

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"