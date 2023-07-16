#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <iostream>
#include <vector>

void passArray(const emscripten::val &floatArrayObject) {
    unsigned int length = floatArrayObject["length"].as<unsigned int>();
    std::vector<float> floatArray;
    floatArray.resize(length);
    auto memory = emscripten::val::module_property("HEAPU8")["buffer"];
    auto memoryView = floatArrayObject["constructor"].new_(memory, reinterpret_cast<uintptr_t>(floatArray.data()), length);
    memoryView.call<void>("set", floatArrayObject);

    for (auto &floatValue : floatArray) {
        std::cout << floatValue << ", ";
    }
    std::cout << std::endl;
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("passArray", &passArray);
}
