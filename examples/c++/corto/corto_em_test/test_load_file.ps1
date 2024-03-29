#Set-PSDebug -Trace 1
$source_file = "test_load_file"
copy load_corto_mesh.js ./../serve
emcc "$($source_file).cpp" load_corto_mesh.cpp -o "./../serve/$($source_file).html" -s WASM=1 --shell-file "$($source_file).html" -s EXPORTED_FUNCTIONS='["_malloc", "_free", "_main"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
start "http://localhost:8000/$($source_file).html"
python3 -m http.server --directory ./../serve