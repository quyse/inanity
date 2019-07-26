/* конфигурационный файл libpng
 * для компиляции в составе с inanity2
 * написан вручную, в основном переводом из файла projects/vstudio/libpng/libpng.vcxproj.
 */

var macros = [];

var includes = ['../zlib'];

exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile(a[2].replace(/\./g, '/') + '.c');
	for ( var i = 0; i < macros.length; ++i)
		compiler.addMacro(macros[i]);
	for ( var i = 0; i < includes.length; ++i)
		compiler.addIncludeDir(includes[i]);
	compiler.cppMode = false;
};

var library = {
	objects: [
	'png',
	'pngerror',
	'pngget',
	'pngmem',
	'pngpread',
	'pngread',
	'pngrio',
	'pngrtran',
	'pngrutil',
	'pngset',
	'pngtrans',
	'pngwio',
	'pngwrite',
	'pngwtran',
	'pngwutil',
	],
	'objects-android': [
	'arm.arm_init',
	'arm.filter_neon_intrinsics',
	],
	'objects-switch': [
	'arm.arm_init',
	'arm.filter_neon_intrinsics',
	],
};

var platformed = function(object, field, platform) {
	return (object[field] || []).concat(object[field + '-' + platform] || []);
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = platformed(library, 'objects', composer.platform);
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
