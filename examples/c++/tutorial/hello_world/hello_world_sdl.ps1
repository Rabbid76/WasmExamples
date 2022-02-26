$source = "hello_world_sdl"
new-item "./bin/$($source)" -force -itemtype directory | Out-Null
em++ -s WASM=1 "$($source).cpp" -o "./bin/$($source)/$($source).html"
start "http://localhost:8000/$($source).html"
python -m http.server --directory "./bin/$($source)"