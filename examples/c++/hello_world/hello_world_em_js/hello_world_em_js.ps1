#Set-PSDebug -Trace 1

$source = "hello_world_em_js"

emcc --no-entry -O0 "$($source).cpp" -o "$($source).js" -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"