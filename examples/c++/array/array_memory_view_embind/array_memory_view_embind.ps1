#Set-PSDebug -Trace 1

$source = "array_memory_view_embind"

emcc --no-entry --bind -O0 "$($source).cpp" -o "$($source).js" -s WASM=1

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"