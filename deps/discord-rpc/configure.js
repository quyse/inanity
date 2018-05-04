exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile('repo/src/' + a[2] + '.cpp');
	compiler.addIncludeDir('repo/include');
	compiler.addIncludeDir('../rapidjson/include');
	compiler.strict = false;
};

var config = {
	objects: 'discord_rpc rpc_connection serialization'.split(' '),
	'objects-win32': 'connection_win discord_register_win'.split(' '),
	'objects-linux': 'connection_unix discord_register_linux'.split(' '),
	'objects-darwin': 'connection_unix discord_register_osx'.split(' ')
};

var platformed = function(object, field, platform) {
	return (object[field] || []).concat(object[field + '-' + platform] || []);
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = platformed(config, 'objects', composer.platform);
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
