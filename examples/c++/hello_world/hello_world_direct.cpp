#include <emscripten.h>

extern "C" {

    float lerp1(float a, float b, float c) {
        return a*(1.0f-c) + b*c;
    }

    EMSCRIPTEN_KEEPALIVE float lerp2(float a, float b, float c) {
        return a*(1.0f-c) + b*c;
    }
}
