#include <emscripten.h>

extern "C" {

    EMSCRIPTEN_KEEPALIVE void run() {
        emscripten_run_script("callback(\"Hello World\")");
    }
}