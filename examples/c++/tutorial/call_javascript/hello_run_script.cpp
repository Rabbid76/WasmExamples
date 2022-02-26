// Calling JavaScript from C/C++
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-javascript-from-c-c

#include <cmath>
#include <iostream>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE void test_run_script() {
    emscripten_run_script("print_to_output('test emscripten_run_script')");
}

EMSCRIPTEN_KEEPALIVE void output(const char *message) {
    std::cout << message << std::endl;
}

int main() {
    output("start ..\n");
    return 0;
}