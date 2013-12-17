/* configure for building zlib */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile(a[2] + '.c');
	compiler.cppMode = false;
};

var objects = 'adler32 compress crc32 deflate infback inflate inftrees inffast trees uncompr zutil'.split(' ');
// 'gzclose gzlib gzread gzwrite' are excluded

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	for(var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
