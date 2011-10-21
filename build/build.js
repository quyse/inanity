var icecpp = require('ice-cpp');

if(process.platform == 'win32')
	icecpp.addEnvFile('build/env');
icecpp.addMacro('INANITY_LIB');
icecpp.makeLibrary('debug/libinanity2');
