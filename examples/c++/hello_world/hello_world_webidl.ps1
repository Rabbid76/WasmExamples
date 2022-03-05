$emscripten_tools = "C:/source/emscripten/emsdk/upstream/emscripten/tools"
$source = "hello_world_webidl"
$target = "hello_world_webidl_"

python3 "$($emscripten_tools)/webidl_binder.py" "$($source).idl" $target

emcc --no-entry -O0 "$($source).cpp" -o "$($source).js" -s WASM=1 --post-js "$($target).js"

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"