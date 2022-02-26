
async function loadWasm(wasmFilename, callBack, importObject) {

    let response = await fetch(wasmFilename);
    let bytes = await response.arrayBuffer();
    let module = await WebAssembly.compile(bytes);
    let imports = WebAssembly.Module.imports(module);
    console.log(WebAssembly.Module.exports(module));
    console.log(imports);
  
    if (!importObject) {
        importObject = {}
    }
    imports.forEach(element => {
        if (element.kind == 'function') {
            if (!importObject[element.module]) {
                console.log('missing module: ' + element.module); 
                importObject[element.module] = {}
            } 
            if (!importObject[element.module][element.name]) {
                console.log('missing function: ' + element.module + '.' + element.name); 
                importObject[element.module][element.name] = (args) => console.log(args);
            } 
        }
    });


    let instance = await WebAssembly.instantiate(module, importObject);
    //instance.bind = {};
    //await instance.exports.__embind_register_native_and_builtin_types(instance.bind);
    //await instance.exports.__wasm_call_ctors(instance.bind);
    callBack(instance);
    
}

loadWasm('hello_world_test1.wasm', (instance) => {
    let text = instance.exports._Z6outputv();
    console.log(text);
});


