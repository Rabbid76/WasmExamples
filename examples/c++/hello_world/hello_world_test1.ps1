#Set-PSDebug -Trace 1

$source = "array_memory_view_cwrap"

#new-item "./bin/$($source)" -force -itemtype directory | Out-Null

emcc -O0 "$($source).cpp" -o "$($source).wasm" -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'
#emcc -bind -O0 "$($source).cpp" -o "$($source).wasm" -s WASM=1 -s LLD_REPORT_UNDEFINED -s EXPORTED_FUNCTIONS=__wasm_call_ctors,__embind_register_native_and_builtin_types

start "http://localhost:8090/$($source).html"
python -m http.server 8090 --directory "./"