#include <emscripten.h>

extern "C" {

    const float lerp(float a, float b, float c) {
        return a*(1.0f-c) + b*c;
    }
}