exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/<api>-object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile(a[2].replace(/\./g, '/') + '.cpp');
};

var staticLibraries = [
	'libinanity-platform',
	{ lib: 'libinanity-dx11', platform: 'win32' },
	'libinanity-gl',
	'libinanity-graphics',
	'libinanity-platform',
	'libinanity-shaders',
	'libinanity-input',
	'libinanity-bullet',
	'libinanity-physics',
	'libinanity-crypto',
	'libinanity-sqlitefs',
	'libinanity-compress',
	'libinanity-base',
	'libinanity-meta',
	'libinanity-lua'
];
var staticDepsLibraries = [
	{ dir: 'lua', lib: 'liblua' },
	{ dir: 'bullet', lib: 'libbullet-dynamics' },
	{ dir: 'bullet', lib: 'libbullet-collision' },
	{ dir: 'bullet', lib: 'libbullet-linearmath' },
	{ dir: 'libpng', lib: 'libpng' },
	{ dir: 'glew', lib: 'libglew' },
	{ dir: 'sqlite', lib: 'libsqlite' }
];
var dynamicLibraries = {
	win32: [
		'user32.lib', 'gdi32.lib', 'opengl32.lib'
	],
	linux: [
		'pthread', 'GL', 'X11', 'dl', 'z', 'xcb', 'X11-xcb'
	]
};

var executables = {
	'quad': [ 'quad', 'ExampleGame' ]
};

exports.configureLinker = function(executableFile, linker) {
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(executableFile);
	linker.configuration = a[2];

	var objects = executables[a[3]];
	for ( var i = 0; i < objects.length; ++i)
		linker.addObjectFile(a[1] + objects[i]);

	for(var i = 0; i < staticLibraries.length; ++i) {
		var lib = undefined;
		if(typeof staticLibraries[i] == 'string')
			lib = staticLibraries[i];
		else if(staticLibraries[i].platform == linker.platform)
			lib = staticLibraries[i].lib;
		if(lib)
			linker.addStaticLibrary('../' + a[1] + lib);
	}
	for(var i = 0; i < staticDepsLibraries.length; ++i)
		linker.addStaticLibrary('../deps/' + staticDepsLibraries[i].dir + '/' + a[1] + staticDepsLibraries[i].lib);

	var dl = dynamicLibraries[linker.platform];

	for(var i = 0; i < dl.length; ++i)
		linker.addDynamicLibrary(dl[i]);
};
