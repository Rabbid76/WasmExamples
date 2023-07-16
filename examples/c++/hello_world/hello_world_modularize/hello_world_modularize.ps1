#Set-PSDebug -Trace 1

$source = "hello_world_modularize"

emcc --no-entry --bind -O0 "$($source).cpp" -o "$($source).js" -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="createMyModule"

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"