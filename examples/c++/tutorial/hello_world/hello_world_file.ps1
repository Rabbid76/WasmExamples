$source = "hello_world_file"
new-item "./bin/$($source)" -force -itemtype directory | Out-Null
copy-item "$($source).txt" -destination "./bin/$($source)"
em++ -s WASM=1 "$($source).cpp" -o "./bin/$($source)/$($source).html" --preload-file "$($source).txt"
start "http://localhost:8000/$($source).html"
python -m http.server --directory "./bin/$($source)"