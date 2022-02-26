# name of the source file
$source = "hello_world"

# create output directory
new-item "./bin/$($source)" -force -itemtype directory | Out-Null

# compile and create js, wasm and html
emcc -s WASM=1 "$($source).c" -o "./bin/$($source)/$($source).html"

# python built-in local webserver 
start "http://localhost:8000/$($source).html"
python3 -m http.server --directory "./bin/$($source)"
