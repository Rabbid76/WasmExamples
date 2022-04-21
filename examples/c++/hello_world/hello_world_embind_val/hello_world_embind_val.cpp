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
