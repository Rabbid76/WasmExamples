#include <cmath>
#include <iostream>
#include <emscripten.h>

extern void decode_model_form_ptr(void* buffer, uint32_t buffer_size);

extern "C" {

EMSCRIPTEN_KEEPALIVE void print_cout(const char *message) {
    std::cout << message << std::endl;
}

EMSCRIPTEN_KEEPALIVE void send_mesh_buffer(void* buffer, uint32_t buffer_size) {
    
    std::cout << "address: " << buffer << " bytes: " << buffer_size << std::endl;
    decode_model_form_ptr(buffer, buffer_size);
} 

}

int main() {
    print_cout("start ...\n");
    return 0;
}