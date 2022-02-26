// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

// Call compiled C/C++ code “directly” from JavaScript
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#call-compiled-c-c-code-directly-from-javascript

#include <cmath>
#include <iostream>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE double calc_sqrt(double value) {
    return std::sqrt(value);
}

EMSCRIPTEN_KEEPALIVE void output(const char *message) {
    std::cout << message << std::endl;
}

int main() {
    output("start ..\n");
    return 0;
}