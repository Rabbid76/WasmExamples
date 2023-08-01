#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <iostream>

std::string hello();
std::string world();
std::string helloWorld();

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("hello", &hello);
   emscripten::function("world", &world);
   emscripten::function("helloWorld", &helloWorld);
}
