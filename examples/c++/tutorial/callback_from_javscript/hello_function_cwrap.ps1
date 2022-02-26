$source = "hello_function_cwrap"

new-item "./bin/$($source)" -force -itemtype directory | Out-Null
emcc "$($source).cpp" -o "./bin/$($source)/$($source).html" -s EXPORTED_FUNCTIONS='["_int_sqrt"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s WASM=1 --shell-file "$($source).html"
start "http://localhost:8000/$($source).html"
python -m http.server --directory "./bin/$($source)"