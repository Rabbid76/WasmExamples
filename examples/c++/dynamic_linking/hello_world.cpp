#include <string>

std::string hello();

std::string world() {
   return "World";
}

std::string helloWorld() {
   return hello() + ", " + world() + "!";
}