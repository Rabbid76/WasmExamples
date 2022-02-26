$source = "hello_world"

#new-item "./bin/$($source)" -force -itemtype directory | Out-Null
emcc --bind -O0 "$($source).cpp" -o "$($source).js" -s WASM=1 -s EXPORTED_FUNCTIONS='["_get_text_c","_print_text_c"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

start "http://localhost:8071/$($source).html"
python -m http.server 8071 --directory "./"