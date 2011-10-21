var icecpp = require('ice-cpp');

if(process.platform == 'win32')
	icecpp.addEnvFile('build/env');
icecpp.addMacro('INANITY_LIB');
icecpp.addIncludeDir('deps/libuv/include');
icecpp.addIncludeDir('deps/lua-5.1.4/src');
icecpp.makeLibrary('debug/libinanity2');
