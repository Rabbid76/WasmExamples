$source = "emscripten_opengl_egl_es2"
emcc "$($source).cpp" -o "$($source).html" -s FULL_ES2=1
start "http://localhost:8000/$($source).html"
python -m http.server