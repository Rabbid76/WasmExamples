#include <emscripten.h>

EM_JS(void, em_js_callback, (), {
    let message = "Hello World";
    console.log(message);
    document.getElementById("output").innerHTML = message;
});

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE void run() {
        em_js_callback();
    }
}
