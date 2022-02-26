$source_file = "bind_load_file"
emcc --bind "$($source_file).cpp" -o "./../serve/$($source_file).html" -s WASM=1 --shell-file "$($source_file).html"
start "http://localhost:8000/$($source_file).html"
python3 -m http.server --directory ./../serve