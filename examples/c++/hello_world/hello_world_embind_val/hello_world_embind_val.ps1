#Set-PSDebug -Trace 1

$source = "hello_world_embind_val"

emcc --no-entry --bind -O0 "$($source).cpp" -o "$($source).js" -s WASM=1

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"