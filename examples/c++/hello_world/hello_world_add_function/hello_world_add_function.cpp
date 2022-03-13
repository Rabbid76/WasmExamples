#include <stdlib.h>
#include <emscripten.h>

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE void run(char *callback_ptr) {
        int fp = atoi(callback_ptr);
        void (*f)(int) = reinterpret_cast<void (*)(int)>(fp);
        f(0);
    }
}