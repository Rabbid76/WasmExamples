$port = 8094

wasm-pack build --target web
echo http://localhost:$port
python3 http_server.py $port