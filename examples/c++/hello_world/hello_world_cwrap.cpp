#include <emscripten.h>

extern "C" {

    EMSCRIPTEN_KEEPALIVE const char *getText() {
        return "Hello World";
    }
}
