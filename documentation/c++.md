# C++ Emscripten examples

Emscripten examples with C++

Subpages:

- [C/C++ binding](emscripten/binding-c++.md)
- [C/C++ language features](emscripten/c++_language_features.md)

## Binding c++

## Resources

- [Emscritpen](https://emscripten.org/)
- [WebAssembly](https://webassembly.org/)  
- [Download and install](https://emscripten.org/docs/getting_started/downloads.html)
- [Emscripten/HTML integration tips](https://floooh.github.io/2017/02/22/emsc-html.html)
- [WebAssembly Experiments](https://floooh.github.io/2016/03/17/oryol-webassembly.html)
- [WebAssembly Demystified](https://floooh.github.io/2017/06/09/webassembly-demystified.html)
- [Alon Zakai — Shipping Tiny WebAssembly Builds](https://www.youtube.com/watch?v=_lLqZR4ufSI)

## Instantiate WASM

- [WebAssembly Concepts](https://developer.mozilla.org/en-US/docs/WebAssembly/Concepts)
- [torch2424/wasm-by-example](https://github.com/torch2424/wasm-by-example/blob/master/demo-util/instantiateWasm.js)

## Compile and link options

- [Emscripten Compiler Frontend (emcc)](https://emscripten.org/docs/tools_reference/emcc.html)
- [emscripten-core/emscripten](https://github.com/emscripten-core/emscripten/blob/main/src/settings.js)

## Install

Windows and Powershell:

```lang-none
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
emsdk_env.bat
```

Use python 2.7 on Mac terminal (`pip install certifi`)

```lang-non
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
python ./emsdk install latest
python ./emsdk activate latest
source ./emsdk_env.sh
```

If you change the location of the SDK (e.g. take it to another computer on an USB), re-run the `./emsdk activate latest` and `emsdk_env.bat` commands.

Sanity tests:

```lang-none
emcc --check
``` 

## Example

_hello_world.c_

```c
#include <stdio.h>

int main() {
  printf("hello, world!\n");
  return 0;
}
```

Create _js_

```lang-none
emcc hello_world.c
node a.out.js
```

or create _html_

```lang-none
emcc hello_world.c -o hello_world.html
python -m http.server
```

`http://localhost:8000/hello_world.html`

### Create example with powershell script

*hello_world.ps*:

```ps1
# name of the source file
$source = "hello_world"

# create output directory
new-item "./bin/$($source)" -force -itemtype directory | Out-Null

# compile and create js, wasm and html
emcc -s WASM=1 "$($source).c" -o "./bin/$($source)/$($source).html"

# python built-in local webserver 
start "http://localhost:8000/$($source).html"
python -m http.server --directory "./bin/$($source)"
```

Run powershell script

```ps
.\hello_world.ps1
```

Terminate with Ctrl-C

## OpenGL

[WebGL-friendly subset of OpenGL ES 2.0/3.0](https://emscripten.org/docs/porting/multimedia_and_graphics/OpenGL-support.html#webgl-friendly-subset-of-opengl-es-2-0-3-0)

## Main loop and events

[Browser main loop](https://emscripten.org/docs/porting/emscripten-runtime-environment.html#browser-main-loop)

## Emscripten and OpenCV

[Build OpenCV.js](https://docs.opencv.org/4.x/d4/da1/tutorial_js_setup.html)
