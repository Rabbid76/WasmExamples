#include <cmath>
#include <iostream>
#include <string>
#include <emscripten.h>
//#include <emscripten/val.h>

//using namespace emscripten;

extern "C" {

EMSCRIPTEN_KEEPALIVE void new_mesh(
    float* vertices, uint32_t vertices_length,
    float* normals, uint32_t normals_length,
    float* uv_coordinates, uint32_t uv_length,
    uint32_t* indices, uint32_t index_length) {
    
    std::cout << vertices_length / 3 << " vertice " << (vertices ? std::to_string(vertices[0]) : "") << std::endl;
    std::cout << normals_length / 3 << " normals " << (normals ? std::to_string(normals[0]) : "") << std::endl;
    std::cout << uv_length / 2 << " texture coordinates " << (uv_coordinates ? std::to_string(uv_coordinates[0]) : "") << std::endl;
    std::cout << index_length / 3 << " indices " << (indices ? std::to_string(indices[0]) : "") << std::endl;
} 

}

void decode_model_form_ptr(void* buffer, uint32_t buffer_size) {
    EM_ASM({
        console.log($0, $1);
        let data_array = new Uint8Array(Module.HEAPU8.buffer, $0, $1);
        
        //decode_model(data_array);
        decode_model_ptr(data_array);
    }, buffer, buffer_size);

    //auto decoder = val::global("CortoDecoder").new_();
    //auto geometry = decoder.call<val>("decode", )
}