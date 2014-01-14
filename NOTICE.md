# Licenses of third party libraries.

All used libraries have [permissive open source licenses](http://en.wikipedia.org/wiki/Permissive_free_software_licence) allowing use of their code in a closed-source software.

## Third-party software in the repo

All third-party software are placed under `/deps` directory.

* [zlib](http://zlib.net/) - [zlib license](http://zlib.net/zlib_license.html)
* [lua](http://www.lua.org/) - [MIT license](http://www.lua.org/license.html)
* [libpng](http://www.libpng.org/pub/png/libpng.html) - libpng license (see `/deps/libpng/LICENSE`)
* [libsquish](https://code.google.com/p/libsquish/) - MIT license (see `/deps/libsquish/README`)
* [GLEW](http://glew.sourceforge.net/) - [modified MIT license](http://glew.sourceforge.net/glew.txt)
* [eigen](http://eigen.tuxfamily.org/) - [Mozilla Public License 2.0](http://eigen.tuxfamily.org/index.php?title=Main_Page#License) (LGPL parts of eigen are explicitly excluded by having "`#define EIGEN_MPL2_ONLY`" in `/math/eigen.hpp`)
* [sqlite](http://www.sqlite.org/) - [in public domain](http://sqlite.org/copyright.html)
* [bullet](http://bulletphysics.org) - [zlib license](http://zlib.net/zlib_license.html)
* [http-parser](https://github.com/joyent/http-parser) - MIT license
* [utf8](http://utfcpp.sourceforge.net/) - MIT-like license (see `/deps/utf8.h` heading comment)
* [libogg](http://xiph.org/ogg/) - BSD-style license (see `/deps/libogg/COPYING`)
* [libvorbis](http://xiph.org/vorbis/) - BSD-style license (see `/deps/libvorbis/COPYING`)
* [v8](https://developers.google.com/v8/) - New BSD license (see `/deps/v8/repo/LICENSE`)
* [freetype](http://freetype.org/) - Freetype license (see `/deps/freetype/repo/docs/FTL.TXT`)
* [harfbuzz](http://harfbuzz.org/) - "Old MIT" license (see `/deps/harfbuzz/repo/COPYING`)
* [lz4](https://code.google.com/p/lz4/) - BSD license
* [npapi](https://code.google.com/p/npapi-sdk/) - Mozilla Public License 1.1
* [fcgi](http://www.fastcgi.com) - BSD-style license (see `/deps/fcgi/LICENSE.TERMS`)

## Other used third-party software

These are not placed in the repository, but could be used by various parts of the engine:

* [Boost](http://www.boost.org/) - [Boost license](http://www.boost.org/LICENSE_1_0.txt)
* [SDL2](http://www.libsdl.org/) - [zlib license](http://www.gzip.org/zlib/zlib_license.html)
* [Emscripten](https://github.com/kripken/emscripten) - MIT license
