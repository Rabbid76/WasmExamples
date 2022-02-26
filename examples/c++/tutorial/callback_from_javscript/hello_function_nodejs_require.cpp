// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

// Interacting with an API written in C/C++ from NodeJS
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-an-api-written-in-c-c-from-nodejs

#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void sayHi() {
  printf("Hi!\n");
}

EMSCRIPTEN_KEEPALIVE
int daysInWeek() {
  return 7;
}

/*
Compile the library with emcc:

emcc api_example.c -o api_example.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=ccall


Require the library and call its procedures from node:

var factory = require('./api_example.js');

factory().then((instance) => {
  instance._sayHi(); // direct calling works
  instance.ccall("sayHi"); // using ccall etc. also work
  console.log(instance._daysInWeek()); // values can be returned, etc.
});

What is this JavaScript "require"?
https://stackoverflow.com/questions/9901082/what-is-this-javascript-require

[`require()`][1] is not part of the standard JavaScript API. But in Node.js, it's a built-in function with a special purpose: [to load modules][2].
*/