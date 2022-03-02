# Binding C/C++

[Interacting with code](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-binding-cpp)

## Using direct function calls

[Call compiled C/C++ code “directly” from JavaScript](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#call-compiled-c-c-code-directly-from-javascript)

Functions in the original source become JavaScript functions that can be called directly. However, the types must be converted manually.

Example _lerp.cpp_:

```c++
#include <emscripten.h>

extern "C" {

    const float lerp(float a, float b, float c) {
        return a*(1.0f-c) + b*c;
    }
}
```

It is sufficient to generate _wasm_. The exported functions must be specified with the full name and a leading `_` with the EXPORTED_FUNCTIONS option.
To build in STANDALONE_WASM mode without a main(), use emcc --no-entry

```lang-none
emcc --no-entry "lerp.cpp" -o "lerp.wasm" -s WASM=1 -s EXPORTED_FUNCTIONS=_lerp
```

Load the _wasm_ and compile and instantiate the WebAssembly code with [WebAssembly.instantiate()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) or [WebAssembly.instantiateStreaming()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiateStreaming): 

```js
async function loadWasm(wasmFilename, callBack, importObject) {

    let response = await fetch(wasmFilename);
    let bytes = await response.arrayBuffer();
    let module = await WebAssembly.compile(bytes);
    let imports = WebAssembly.Module.imports(module);
    console.log(WebAssembly.Module.exports(module));
    console.log(imports);
    importObject = importObject || {};
    imports.forEach(element => {
        if (element.kind == 'function') {
            if (!importObject[element.module]) {
                console.log('missing module: ' + element.module); 
                importObject[element.module] = {}
            } 
            if (!importObject[element.module][element.name]) {
                console.log('missing function: ' + element.module + '.' + element.name); 
                importObject[element.module][element.name] = (args) => console.log(args);
            } 
        }
    });
    let instance = await WebAssembly.instantiate(module, importObject);
    callBack(instance);
}

loadWasm('lerp.wasm', (instance) => {
    let result = instance.exports.lerp(1, 2, 0.5);
    console.log(result);
});
```

## Using ccall or cwrap

[Calling compiled C functions from JavaScript using ccall/cwrap](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap)

## Embind

[Embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html)

## WebIDL

[WebIDL](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html#webidl-binder)
