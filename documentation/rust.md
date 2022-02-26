# WebAssembly with Rust

## Resources

### Rust WebAssembly

- [Crate wasm_bindgen](https://docs.rs/wasm-bindgen/latest/wasm_bindgen/)
- [GitHub - rustwasm/wasm-bindgen](https://github.com/rustwasm/wasm-bindgen)
- [wasm-bindgen](https://crates.io/crates/wasm-bindgen)  
- [Rust - Web­Assembly](https://www.rust-lang.org/what/wasm)
- [Compiling from Rust to WebAssembly](https://developer.mozilla.org/en-US/docs/WebAssembly/Rust_to_wasm)  
- [Shrinking .wasm Size](https://rustwasm.github.io/book/game-of-life/code-size.html#shrinking-wasm-size)
- [Rust 🦀 and WebAssembly 🕸](https://rustwasm.github.io/book/#who-is-this-book-for)

### Emscripten and Rust

- [wasmer-emscripten](https://crates.io/crates/wasmer-emscripten)  
- [Crate wasmer_emscripten](https://docs.rs/wasmer-emscripten/latest/wasmer_emscripten/)
- [Small WebAssembly Binaries with Rust + Emscripten](https://kripken.github.io/blog/binaryen/2018/04/18/rust-emscripten.html)
- [Hello, Rust!](https://www.hellorust.com/setup/emscripten/)
- [Wasm By Example](https://wasmbyexample.dev/examples/reading-and-writing-graphics/reading-and-writing-graphics.rust.en-us.html)
- [zaru/webassembly-demo](https://github.com/zaru/webassembly-demo)

## Install

### wasm-pack

Install wasm-pack

```lang-none
cargo install wasm-pack
```

Windows issue with MySys:

>```lang-none
>This perl implementation doesn't produce Windows like paths (with backward
>slash directory separators).  Please use an implementation that matches your
>building platform.
>```

Install Perl: [Strawberry Perl](https://strawberryperl.com/)

### Emscripten

...

## Create project

### Rust wasm with crate wasm-bindgen

Create lib:

```rust
use wasm_bindgen::prelude::*;

#[wasm_bindgen]
extern {
    pub fn alert(s: &str);
}

#[wasm_bindgen]
pub fn greet(name: &str) {
    alert(&format!("Hello, {}!", name));
}
```

Build:

```lang-none
wasm-pack build --target web
```

Example html:

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>hello-wasm example</title>
  </head>
  <body>
    <script type="module">
      import init, {greet} from "./pkg/hello_wasm.js";
      init()
        .then(() => {
          greet("WebAssembly")
        });
    </script>
  </body>
</html>
```

Run with simple python http server

```py
import http.server
import socketserver
import sys

PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8000

Handler = http.server.SimpleHTTPRequestHandler
Handler.extensions_map.update({
      ".js": "application/javascript",
})

httpd = socketserver.TCPServer(("", PORT), Handler)
httpd.serve_forever()
```

```

### Rust Emscripten

...
