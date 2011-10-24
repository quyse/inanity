/* конфигурационный файл lua
 * для компиляции в составе с inanity2
 * написан вручную, по инструкциям из INSTALL
 */

exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^[^\/]+\/([^\/]+)$/.exec(objectFile);
	compiler.setSourceFile('src/' + a[1] + '.c');
};

var objects = 'lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ldblib liolib lmathlib loslib ltablib lstrlib loadlib linit' //
.split(' ');

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^([^\/]+\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
