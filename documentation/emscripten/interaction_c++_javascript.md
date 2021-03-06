# Interaction between C/C++ and JavaScript

[Interacting with code](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-binding-cpp)  
[API Reference](https://emscripten.org/docs/api_reference/index.html)  

## Call C/C++ code “directly”

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

## Calling C++ with `ccall` or `cwrap`

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

## Inline JavaScript in C/C++

[Calling JavaScript from C/C++](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-call-javascript-from-native)

`emscripten_run_script()` can be used to run inline JavaScript code in C++ or to call JavaScript functions.

_hello_world.cpp_:

```c++
#include <emscripten.h>

extern "C" {

    EMSCRIPTEN_KEEPALIVE void run() {
        emscripten_run_script("callback(\"Hello World\")");
    }
}
```  

```none
emcc --no-entry hello_world.cpp -o hello_world.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        let result = Module.ccall('run', null, []);
    }
};
function callback(message) {
    console.log(message);
}
</script>
<script src="hello_world.js"></script>
```

## Inline assembly code (`EM_ASM`)

With [`EM_ASM`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-call-javascript-from-native), Emscripten will execute the two lines of JavaScript as if they appeared directly in the generated code.

_hello_world.cpp_:

```c++
#include <emscripten.h>

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE void run() {
        EM_ASM(
            let message = "Hello World";
            console.log(message);
        );
    }
}
```

```none
emcc --no-entry hello_world.cpp -o hello_world.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        let result = Module.ccall('run', null, []);
    }
};
</script>
<script src="hello_world.js"></script>
```

## C API implemented in JavaScript (JavaScript library)

[Implement a C API in JavaScript](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#implement-a-c-api-in-javascript)  
[JavaScript limits in library files](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#javascript-limits-in-library-files)

Define the C API interface as `extern` and implement the javascript function in a separate file. Bind the javascript implementation file using the `--js-library <lib>` option when building the wasm.

_hello_world.cpp_:

```c++
#include <emscripten.h>

extern "C" {

    extern void capi_callback();

    EMSCRIPTEN_KEEPALIVE void run() {
        capi_callback();
    }
}
```

_hello_world_library.js_:

```js
mergeInto(LibraryManager.library, {
    capi_callback: function() {
        let message = "Hello World";
        console.log(message);
    },
});
```

```none
emcc --no-entry hello_world.cpp -o hello_world.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap --js-library _hello_world_library.js
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        let result = Module.ccall('run', null, []);
    }
};
</script>
<script src="hello_world.js"></script>
```

## JavaScript function inside c++ (`EM_JS`)

[`EM_JS`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-call-javascript-from-native)s implementation is essentially a shorthand for implementing a JavaScript library.

_hello_world.cpp_:

```c++
#include <emscripten.h>

EM_JS(void, em_js_callback, (), {
    let message = "Hello World";
    console.log(message);
});

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE void run() {
        em_js_callback();
    }
}
```

```none
emcc --no-entry hello_world.cpp -o hello_world.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        let result = Module.ccall('run', null, []);
    }
};
</script>
<script src="hello_world.js"></script>
```

## JavaScript function pointers

[Calling JavaScript functions as function pointers from C](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-call-function-pointers-from-c)

The `addFunction` Emscripten JsvaScript runtime function returns an integer value that represents a function pointer.
Building with `-s ALLOW_TABLE_GROWTH` allows new functions to be added to the function table.

_hello_world.cpp_:

```c++
#include <stdlib.h>
#include <emscripten.h>

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE void run(char *callback_ptr) {
        int fp = atoi(callback_ptr);
        void (*f)(int) = reinterpret_cast<void (*)(int)>(fp);
        f(0);
    }
}
```

```none
emcc --no-entry hello_world.cpp -o hello_world.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall,addFunction -s ALLOW_TABLE_GROWTH
```

```html
<script>
var Module = {
    onRuntimeInitialized: function() {
        var newFuncPtr = Module.addFunction(function(num) {
            let message = num + ": Hello World";
            console.log(message);
        }, 'vi');
        let result = Module.ccall('run', null, ['string'], [newFuncPtr.toString()]);
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

## Using `val`

[Using val to transliterate JavaScript to C++](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind-val-guide)

> Embind provides a C++ class, `emscripten::val`, which you can use to transliterate JavaScript code to C++.
> Using `val` you can call JavaScript objects from your C++, read and write their properties, or coerce them to C++ values like a `bool`, `int`, or `std::string`

_hello_world.cpp_:

```c++
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdio.h>

void run() {
   emscripten::val moduleContext = emscripten::val::global("moduleContext");
   if (!moduleContext.as<bool>()) {
      printf("No moduleContext\n");
      return;
   }
   printf("Got moduleContext\n");

   moduleContext.set("text", emscripten::val("Hello world"));
   moduleContext.call<void>("setText", 0);
   printf("All done!\n");
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("run", &run);
}

```

```none
emcc --no-entry -lembind hello_world.cpp -o hello_world.js -s WASM=1
```

```html
<script>
var moduleContext = {
    text: "",
    setText: function() {
        console.log(this.text);
        document.getElementById("output").innerHTML = this.text;
    }
}; 
var Module = {
    onRuntimeInitialized: function() {
        Module.run();
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

| option               | description                                          |
|----------------------|------------------------------------------------------|
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

```html
<script src="hello_world_modularize.js"></script>
<script>
createMyModule(/* optional default settings */).then(function(MyModule) {
    let result = MyModule.getText();
    console.log(result);
});
</script>
```
