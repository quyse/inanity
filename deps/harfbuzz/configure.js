/* configure for building harfbuzz
 */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile('repo/src/' + a[2] + '.cc');
	compiler.addIncludeDir('../freetype/repo/include');
	compiler.addIncludeDir('repo/src');
	compiler.addIncludeDir('../ucdn/repo');
	compiler.addIncludeDir('generated');
	compiler.addMacro('HB_NO_MT');
	compiler.addMacro('HAVE_OT');
	compiler.addMacro('HAVE_FREETYPE');
	compiler.addMacro('HAVE_UCDN');
	// crazy hacks for xbox
	if(compiler.platform == 'xbox') {
		compiler.addMacro('getenv=(char*)""+__noop');
		compiler.addMacro('HB_NO_MMAP');
	}

	compiler.strict = false;
};

var objects = [
	// amalgamation file is available since Harfbuzz 2.6.0
	'harfbuzz'
];

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	for(var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
