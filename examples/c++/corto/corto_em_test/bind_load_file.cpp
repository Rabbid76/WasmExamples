#include <cmath>
#include <iostream>
#include <vector>
//#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
//#include <emscripten/wire.h>

using namespace emscripten;

template<typename DATA_TYPE>
void get_buffer_data(std::vector<DATA_TYPE>& data, const val &corto_mesh, const char* property_name) {
    val buffer = corto_mesh[property_name];
    if (buffer.isUndefined()) {
        return;
    }
    val memory = val::module_property("HEAPU8")["buffer"];
    unsigned int buffer_length = buffer["length"].as<unsigned int>();
    data.resize(buffer["length"].as<unsigned int>());
    val memory_view = buffer["constructor"].new_(memory, reinterpret_cast<uintptr_t>(data.data()), buffer_length);
    memory_view.call<void>("set", buffer);
}

void decode_corto_mesh(const val& mesh_array_buffer) {
    std::cout << "test" << std::endl;

    auto corto_decoder = val::global("CortoDecoder");
    val new_corto_mesh = corto_decoder.call<val>("decode", mesh_array_buffer);

    uint32_t no_of_vertices = new_corto_mesh["nvert"].as<uint32_t>();
    uint32_t no_of_indices = new_corto_mesh["nface"].as<uint32_t>();
    std::vector<uint32_t> index_data;
    std::vector<float> vertex_data;
    std::vector<float> normal_data;
    std::vector<float> uv_data;
    get_buffer_data(index_data, new_corto_mesh, "index");
    get_buffer_data(vertex_data, new_corto_mesh, "vertex");
    get_buffer_data(normal_data, new_corto_mesh, "normal");
    get_buffer_data(uv_data, new_corto_mesh, "uv");

    new_corto_mesh.call<void>("dispose");

    std::cout << "number of indices: " << no_of_indices << std::endl;    
    std::cout << "number of vertices: " << no_of_vertices << std::endl;
    if (index_data.size() >= 3)
        std::cout << "indices: [(" << index_data[0] << ", " << index_data[1] << ", " << index_data[2] << "), ...]" << std::endl;
    if (vertex_data.size() >= 3)
        std::cout << "vertices: [(" << vertex_data[0] << ", " << vertex_data[1] << ", " << vertex_data[2] << "), ...]" << std::endl;
    if (normal_data.size() >= 3)
        std::cout << "normals: [(" << normal_data[0] << ", " << normal_data[1] << ", " << normal_data[2] << "), ...]" << std::endl;
    if (uv_data.size() >= 3)
        std::cout << "vertices: [(" << uv_data[0] << ", " << uv_data[1] << ", " << uv_data[2] << "), ...]" << std::endl;
}

namespace roomle {

    class MeshBuffer {
    public:

        std::vector<uint32_t> indices;
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> uvCoords;
    };

    struct TriangularMeshPtr {

    };

    class MeshDecoder {

    public:

        static TriangularMeshPtr createMesh(MeshBuffer& meshBuffer, uint32_t nVert, uint32_t nFace) {

        }
    };
}

namespace {

    struct ConfiguratorKernel {

    };

    template<typename DATA_TYPE>
    void getBufferData(std::vector<DATA_TYPE> &data, const val &cortoMesh, const char *propertyName) {
        val buffer = cortoMesh[propertyName];
        if (buffer.isUndefined()) {
            return;
        }
        val memory = val::module_property("HEAPU8")["buffer"];
        unsigned int bufferLength = buffer["length"].as<unsigned int>();
        data.resize(buffer["length"].as<unsigned int>());
        val memoryView = buffer["constructor"].new_(memory, reinterpret_cast<uintptr_t>(data.data()), bufferLength);
        memoryView.call<void>("set", buffer);
    }

    static void addExternalMeshCorto(
            ConfiguratorKernel &kernel, std::string meshId, int aQuality, const val &typedArray) {

        auto cortoDecoder = val::global("CortoDecoder");
        val meshGeometry = cortoDecoder.call<val>("decode", typedArray);

        uint32_t noOfVertices = meshGeometry["nvert"].as<uint32_t>();
        uint32_t noOfIndices = meshGeometry["nface"].as<uint32_t>();
        roomle::MeshBuffer meshBuffer;
        getBufferData(meshBuffer.indices, meshGeometry, "index");
        getBufferData(meshBuffer.vertices, meshGeometry, "vertex");
        getBufferData(meshBuffer.normals, meshGeometry, "normal");
        getBufferData(meshBuffer.uvCoords, meshGeometry, "uv");
        meshGeometry.call<void>("dispose");

        roomle::TriangularMeshPtr newMesh = roomle::MeshDecoder::createMesh(meshBuffer, noOfVertices, noOfIndices);
        //kernel.addMesh(meshId, (roomle::meshCache::QualityLevel) aQuality, newMesh);
    }
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("decode_corto_mesh", &decode_corto_mesh);
}