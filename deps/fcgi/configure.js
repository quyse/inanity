/* configure for building fcgi */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	var source = a[2];
	var b = /^(.*)\.c$/.exec(source);
	if(b) {
		// .c
		source = b[1].replace(/\./g, '/') + '.c';
		compiler.cppMode = false;
	}
	else
		// .cpp
		source = source.replace(/\./g, '/') + '.cpp';
	compiler.setSourceFile(source);
	compiler.strict = false;
};

var lib = {
	objects: ['fcgi_stdio.c', 'fcgiapp.c', 'fcgio'],
	'objects-win32': ['os_win32.c'],
	'objects-linux': ['os_unix.c']
};

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = lib.objects;
	for(var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
	var platformObjects = lib['objects-' + composer.platform] || [];
	for(var i = 0; i < platformObjects.length; ++i)
		composer.addObjectFile(confDir + platformObjects[i]);
};
