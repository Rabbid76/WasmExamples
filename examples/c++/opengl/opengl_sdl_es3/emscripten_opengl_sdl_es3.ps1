$source = "emscripten_opengl_sdl_es3"
emcc "$($source).cpp" -o "$($source).html" -s FULL_ES3=1
start "http://localhost:8000/$($source).html"
python -m http.server