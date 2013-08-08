exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.cppMode = false;
	compiler.setSourceFile(a[2] + '.c');
	if(compiler.platform != 'win32')
		compiler.addMacro('SQLITE_HAVE_ISNAN');
};

var libraries = {
	'libsqlite': {
		objects: ['sqlite3']
	}
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var library = libraries[a[3]];
	for ( var i = 0; i < library.objects.length; ++i)
		composer.addObjectFile(confDir + library.objects[i]);
};
