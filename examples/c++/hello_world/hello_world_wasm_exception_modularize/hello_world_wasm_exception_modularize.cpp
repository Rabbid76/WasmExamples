#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <iostream>

std::string testTryCatch() {
   std::cout << "Hello world - before try" << std::endl;
   try {
      std::cout << "Hello world in try" << std::endl;
      throw;
   } catch (...) {
      std::cout << "Hello world - in catch" << std::endl;
   }
   std::cout << "Hello world - after catch" << std::endl;
   return "Hello world";
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("testTryCatch", &testTryCatch);
}
