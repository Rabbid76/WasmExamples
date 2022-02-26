$source = "hello_world_sdl_minimal_shell"
new-item "./bin/$($source)" -force -itemtype directory | Out-Null
em++ "$($source).cpp" -o "./bin/$($source)/$($source).html" -s WASM=1 --shell-file ../../shell/shell_minimal.html
start "http://localhost:8000/$($source).html"
python -m http.server --directory "./bin/$($source)"