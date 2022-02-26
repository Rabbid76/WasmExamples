$source = "hello_run_script"

new-item "./bin/$($source)" -force -itemtype directory | Out-Null
emcc "$($source).cpp" -o "./bin/$($source)/$($source).html" -s WASM=1 --shell-file "$($source).html" -s EXPORTED_FUNCTIONS='["_malloc", "_free", "_main"]'
start "http://localhost:8000/$($source).html"
python -m http.server --directory "./bin/$($source)"