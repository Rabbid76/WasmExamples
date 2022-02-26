#include <emscripten.h>
#include <emscripten/em_macros.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <iostream>

extern "C" {

const char * get_text_c() {
    return "Hello world - C";
}

void print_text_c(const char *text) {
   printf("%s\n", text);
}

}

// __Z15get_text_directv
EMSCRIPTEN_KEEPALIVE const char * get_text_direct() {
    return "Hello world - C++ direct";
}

// __Z17print_text_directPKc
EMSCRIPTEN_KEEPALIVE void print_text_direct(const char *text) {
    std::cout << text << std::endl;
}

// __Z17run_script_directv
EMSCRIPTEN_KEEPALIVE void run_script_direct() {
    emscripten_run_script("set_direct('Hello world - C++ run direct')");
}

std::string get_text() {
   return "Hello world - C++";
}

void print_text(std::string text) {
   std::cout << text << std::endl;
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("get_text", &get_text);
   emscripten::function("print_text", &print_text);
}

int main() {
    return 0;
}