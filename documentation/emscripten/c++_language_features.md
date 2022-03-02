# C++ language features and Emscripten

## Exception handling

By default, exception catching is disabled in Emscripten. ([C++ exceptions support](https://emscripten.org/docs/porting/exceptions.html#c-exceptions-support))

Options to opt-in exception handling:

- Compile time and link time option `-fexceptions` (high overhead)  
  [JavaScript-based exception support](https://emscripten.org/docs/porting/exceptions.html#javascript-based-exception-support)

- Compile time and link time option `-fwasm-exceptions` ([not yet supported by default in most engines](https://webassembly.org/roadmap/))  
  [WebAssembly exception handling proposal](https://emscripten.org/docs/porting/exceptions.html#webassembly-exception-handling-proposal)  
  [WebAssembly feature detection](https://web.dev/webassembly-feature-detection/)  

- "Emulated" exception handling with `-s DISABLE_EXCEPTION_CATCHING=0`  
  [C++ exceptions](https://emscripten.org/docs/optimizing/Optimizing-Code.html#c-exceptions)  
  [Exception handling in emscripten: how it works and why it’s disabled by default](https://brionv.com/log/2019/10/24/exception-handling-in-emscripten-how-it-works-and-why-its-disabled-by-default/)

## RTTI - RunTime Type Information

Compiling with `-fn-rtti` causes an error for any use of `dynamic_cast`.
Anyway `dynamic_cast` are not known to be “speedy" and should be reduced.

## Emscripten-specific tuning

- [10 simple diet tricks for asm.js](https://floooh.github.io/2016/08/27/asmjs-diet.html)

What makes the code slow and and the image size large:

- exceptions handling
- `dynamic_cast` and RTTI
- large inline methods
- Polymorphism (`virtual`methods)
- templates
- 64-bit integer data types because they are emulated