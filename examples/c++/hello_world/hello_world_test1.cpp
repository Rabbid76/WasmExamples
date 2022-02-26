#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>

std::string test_text() {
   return "Hello world";
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("test_text", &test_text);
}

EMSCRIPTEN_KEEPALIVE const char* output() {
    return "Hello World";
}

int main() {
    output();
    return 0;
}
