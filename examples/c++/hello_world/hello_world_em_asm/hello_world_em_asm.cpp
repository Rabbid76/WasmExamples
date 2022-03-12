#include <emscripten.h>

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE void run() {
        EM_ASM(
            let message = "Hello World";
            console.log(message);
            document.getElementById("output").innerHTML = message;
        );
    }
}