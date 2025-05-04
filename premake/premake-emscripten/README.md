# Premake Extension to support Emscripten

> [!WARNING]
> This extension is now deprecated, given Emscripten support has been merged in Premake core.

This is a fork of https://github.com/TurkeyMan/premake-emscripten with a bunch of changes:

* `wasm32` and `wasm64` architectures 
* `.wasm` target extension
* `gmake` and `gmake2` actions
* `emscripten` system
* extension preload for `--os=emscripten` command line option support
* fix `emcc` toolset validation in gmake actions
* support for `ninja` build action via [premake-ninja](https://github.com/jimon/premake-ninja)

And the original:

* VisualStudio support through [vs-tool](https://github.com/juj/vs-tool)
