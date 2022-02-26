# GNU make [https://www.gnu.org/software/make/manual/make.html]
# makefile:4: *** missing separator. Stop [https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop]
# Create directories using make file [https://stackoverflow.com/questions/1950926/create-directories-using-make-file]
# Can I search directory and subdirectories for header files? [https://stackoverflow.com/questions/19738956/can-i-search-directory-and-subdirectories-for-header-files]
# How to run pre- and post-recipes for every target using GNU Make? [https://stackoverflow.com/questions/37952098/how-to-run-pre-and-post-recipes-for-every-target-using-gnu-make]


filePathName = $(fileDirname)/$(fileBasenameNoExtension)
filePathNameExt = $(fileDirname)/$(fileBasename)
dirNameOut = $(workspaceRoot)/bin
filePathNameOut = $(dirNameOut)/$(fileBasenameNoExtension)

# [Emscripten](https://emscripten.org/)  
#emscriptenDir = C:\source\WebAssembly\emsdk\emscripten\1.38.29
emscriptenDir = ${EMSCRIPTEN}
emscriptenInc = -I${emscriptenDir}/include

INCLUDEALL = $(emscriptenInc)

OPTIONS = 
#OPTIONS = -s WASM=1
#OPTIONS = -s WASM=1 -s USE_WEBGL2=1

all: target	$(filePathNameOut).html

$(filePathNameOut).html: $(filePathNameExt)
	em++ $(OPTIONS) -o $@ $< $(INCLUDEALL)

target: ${dirNameOut}

${dirNameOut}:
	mkdir -p $@