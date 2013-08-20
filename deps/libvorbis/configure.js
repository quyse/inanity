/* конфигурационный файл libvorbis
 * для компиляции в составе с inanity2
 */

var macros = [];

var includes = ['lib'];

exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile('lib/' + a[2].replace(/\./g, '/') + '.c');
	for ( var i = 0; i < macros.length; ++i)
		compiler.addMacro(macros[i]);
	for ( var i = 0; i < includes.length; ++i)
		compiler.addIncludeDir(includes[i]);
	compiler.cppMode = false;
};

var librariesObjects = {
	libvorbis: [
		"analysis", "bitrate", "block", "codebook", "envelope",
		"floor0", "floor1", "info", "lookup", "lpc", "lsp",
		"mapping0", "mdct", "psy", "registry", "res0",
		"sharedbook", "smallft", "synthesis", "vorbisenc", "window"
	],
	libvorbisfile: ['vorbisfile']
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = librariesObjects[a[3]];
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i].replace(/\//g, '.'));
};
