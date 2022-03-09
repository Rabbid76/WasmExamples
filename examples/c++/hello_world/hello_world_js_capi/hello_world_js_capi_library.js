mergeInto(LibraryManager.library, {
    capi_callback: function() {
        let message = "Hello World";
        console.log(message);
        document.getElementById("output").innerHTML = message;
    },
});