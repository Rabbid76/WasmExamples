#include <emscripten.h>

extern "C" {

    extern void capi_callback();

    EMSCRIPTEN_KEEPALIVE void run() {
        capi_callback();
    }
}
