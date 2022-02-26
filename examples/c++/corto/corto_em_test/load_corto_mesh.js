// see Emscripten and float arrays v2
// https://gist.github.com/jonathanlurie/7e7b2b3aae7e949d19e0a3082d65f49c
//
// Access memory from JavaScript
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#access-memory-from-javascript

class DataBuffer {

    constructor(data) {
        this.length = data ? data.length : 0;
        this.number_of_bytes = data ? data.length * data.BYTES_PER_ELEMENT : 0;
        if (this.number_of_bytes == 0) {
            this.data = null;
            return;
        } 
        let data_ptr = Module._malloc(this.number_of_bytes);
        this.data_heap = new Uint8Array(Module.HEAPU8.buffer, data_ptr, this.number_of_bytes);
        this.data_heap.set(new Uint8Array(data.buffer));
        this.data = this.data_heap.byteOffset;
    }

    free() {
        _free(this.data);
    }
}

var decode_model = function (data_buffer) {
    var model = CortoDecoder.decode(data_buffer, false, false);
    
    console.log(model.nvert, model.nface, model.groups);
    console.log('Index: ', model.index);
    console.log('Positions: ', model.position);
    console.log('Colors: ', model.color);
    console.log('Nornmals: ', model.normal);
    console.log('Tex coords: ', model.uv);

    new_mesh = Module.cwrap('new_mesh', null, ['number', 'number', 'number', 'number', 'number', 'number', 'number', 'number']);

    let vertex_buffer = new DataBuffer(model.position);
    let normal_buffer = new DataBuffer(model.normal);
    let uv_buffer = new DataBuffer(model.uv);
    let index_buffer = new DataBuffer(model.index);
   
    new_mesh(
        vertex_buffer.data, vertex_buffer.length,
        normal_buffer.data, normal_buffer.length,
        uv_buffer.data, uv_buffer.length,
        index_buffer.data, index_buffer.length); 
    
    vertex_buffer.free();
    normal_buffer.free();
    uv_buffer.free();
    index_buffer.free();
}

var decode_model_ptr = function (data_buffer) {
    let geometry = CortoDecoder.decode_ptr(data_buffer, false, false);

    let new_mesh = Module.cwrap('new_mesh', null, ['number', 'number', 'number', 'number', 'number', 'number', 'number', 'number']);
    new_mesh(
        geometry.pptr, geometry.nvert*3,
        geometry.nptr, geometry.hasNormal ? geometry.nvert*3 : 0,
        geometry.uptr, geometry.hasUv ? geometry.nvert*2 : 0,
        geometry.iptr, geometry.nface*3); 
}
