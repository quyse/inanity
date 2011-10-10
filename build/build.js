var icecpp = require('ice-cpp');

icecpp.addMacro('INANITY_LIB');
icecpp.addIncludeDir('deps/libuv/include');
icecpp.makeLibrary('debug/libinanity2');
