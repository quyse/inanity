# Inanity

Inanity is a cross-platform C++ game engine. It aims to provide full, abstract-from-all low-level solution for developing graphics applications and games.

## Status

The whole engine is work-in-progress, it is far from finished state. There are almost no tools for "end users". The engine is considered as an experimental piece of software for those who doesn't afraid to read the engine's code before using.

## Features

* Builds on Windows and Linux;
* Base framework (memory, filesystems, streams, compressing, threads);
* Fully abstracted graphics pipeline with DirectX 11 and OpenGL bindings;
* Cross-API native C++ shader language;
* Platform bindings for Windows (user input via Raw Input, native windowing);
* Compile-time scripting autobindings to C++ classes ([Lua](http://www.lua.org/) is only supported for now);
* Simple physics (integration with [bullet](http://bulletphysics.org/ bullet)).

## Work-in-progress

* Python and Javascript scripting in addition to Lua;
* Networking framework (via [Boost.Asio](http://www.boost.org/libs/asio/ Boost.Asio));
* Platform bindings for Linux (native user input & X.org windowing);
* 3D sounds and audio streaming via XAudio2 (Windows) or OpenAL.

## License

All the code is mine (Copyright (c) Alexander Bich aka Quyse Lert) except third-party code in `deps` directory. All my code was written from scratch.
* My code is licensed under MIT. See LICENSE file.
* All third-party code allows permissive licensing in some way. See NOTICE file for the full list of libraries and licenses.
