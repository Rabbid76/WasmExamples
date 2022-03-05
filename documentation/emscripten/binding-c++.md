# Binding C/C++

Interaction between C++ and JavaScript

[Interacting with code](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-binding-cpp)

## Using direct function calls

[Call compiled C/C++ code “directly” from JavaScript](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#call-compiled-c-c-code-directly-from-javascript)

Functions in the original source become JavaScript functions that can be called directly. However, the types must be converted manually.  
It is sufficient to generate _wasm_. The exported functions must be specified with the full name and a leading `_` with the `EXPORTED_FUNCTIONS` option.
Alternatively, the macro `EMSCRIPTEN_KEEPALIVE` can be used. To build in STANDALONE_WASM mode without a main(), use emcc --no-entry.  
If the functions are not wrapped in an [`extern "C"`](https://en.cppreference.com/w/cpp/language/language_linkage), the function names become a bit more complicated
(e.g. `float lerp2(float, float, float)` becomes `_Z5lerp2fff`).

_lerp.cpp_:

```c++
#include <emscripten.h>

extern "C" {

    float lerp1(float a, float b, float c) {
        return a*(1.0f-c) + b*c;
    }

    EMSCRIPTEN_KEEPALIVE float lerp2(float a, float b, float c) {
        return a*(1.0f-c) + b*c;
    }
}
```

```none
emcc --no-entry lerp.cpp -o lerp.wasm -s WASM=1 -s EXPORTED_FUNCTIONS=_lerp1
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
    let result1 = instance.exports.lerp1(1, 2, 0.5);
    console.log(result1);
    let result2 = instance.exports._Z5lerp2fff(2, 3, 0.4);
    console.log(result2);
});
```

## Using ccall or cwrap

[Calling compiled C functions from JavaScript using ccall/cwrap](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap)

The easiest way to call compiled __C__ functions from JavaScript is to use `ccall()` or `cwrap()`. These functions are provided by the _js_ file generated with the _wasm_.
The runtime functions to be used must be specified with the `EXPORTED_RUNTIME_METHODS` option. Name-mangled C++ functions won’t work with this runtime functions.

_hello_world.cpp_:

```c++
#include <emscripten.h>

extern "C" {

    EMSCRIPTEN_KEEPALIVE const char *getText() {
        return "Hello World";
    }
}
```

```none
emcc --no-entry hello_world.cpp -o hello_world.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap
```

When the _wasm_ is completely initialized the [`onRuntimeInitialized`](https://emscripten.org/docs/api_reference/module.html#Module.onRuntimeInitialized) callback function is invoked.
The runtime functions are accessible via the `Module` object.

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        let result = Module.cwrap('getText', 'string', [])();
        console.log(result);
    }
};
</script>
<script src="hello_world.js"></script>
```

## Embind

[Embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html)

C++ functions and static methods can be bound using the [`EMSCRIPTEN_BINDINGS`](https://emscripten.org/docs/api_reference/bind.h.html#_CPPv419EMSCRIPTEN_BINDINGS4name) macro.
Linking against the _embind_ library is enabled with the `-lembind` option or the deprecated `--bind` option.

_hello_world.cpp_:

```c++
#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <iostream>

std::string getText() {
   return "Hello world";
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("getText", &getText);
}
```

```none
emcc --no-entry -lembind hello_world.cpp -o hello_world.js -s WASM=1
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        let result = Module.getText();
        console.log(result);
    }
};
</script>
<script src="hello_world.js"></script>
```

## WebIDL

[WebIDL](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html#webidl-binder)  

Create the c++ code and a corresponding WebIDL interface ([Web IDL Standard](https://webidl.spec.whatwg.org/)).
The C++ and WebIdl types must match ([WebIDL types](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html#webidl-types)).

_hello_world.h_:

```c++
#include <string>
#include <iostream>

class Foo {
public:
    char* getText() {
        return (char*)"Hello world";
    }
};
```

_hello_world.idl_:

```idl
interface Foo {
    void Foo();
    DOMString getText();
};
```

Use the [_tools/webidl_binder.py_](https://github.com/emscripten-core/emscripten/blob/main/tools/webidl_binder.py) to generate the corresponding _.cpp_ and _.js_ interfaces.

```none
python3 webidl_binder.py hello_world.idl hello_world_interface
```

Use inter face files (_hello_world_interface.cpp_ _hello_world_interface.js_) to generate the _wasm_.
`--pre-js <file>` and `--post-js <file>` can be used to specify content that will be added before and added after the generated _js_ code.

_hello_world.cpp_:

```cpp
#include "hello_world.h"
#include "hello_world_interface.cpp"
```

```none
emcc --no-entry hello_world.cpp -o _hello_world.js -s WASM=1 --post-js hello_world_interface.js
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        var foo = new Module.Foo();
        let result = foo.getText();
        console.log(result);
    }
};
</script>
<script src="hello_world.js"></script>
```

## Modularize

[How can I tell when the page is fully loaded and it is safe to call compiled functions?](https://emscripten.org/docs/getting_started/FAQ.html#how-can-i-tell-when-the-page-is-fully-loaded-and-it-is-safe-to-call-compiled-functions)

| `MODULARIZE=1`       | Wraps the code in a function that returns a promise. |
| `EXPORT_NAME="name"` | Specifies the export name of the module.             |

_hello_world.cpp_:

```c++
#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <iostream>

std::string getText() {
   return "Hello world";
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("getText", &getText);
}
```

```none
emcc --no-entry --bind hello_world.cpp" -o hello_world.js -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="createMyModule"
```

_hello_world.html_:

```html
<script src="hello_world_modularize.js"></script>
<script>
createMyModule(/* optional default settings */).then(function(MyModule) {
    let result = MyModule.getText();
    console.log(result);
});
</script>
```
