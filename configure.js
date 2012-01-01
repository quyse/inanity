exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile(a[2].replace(/\./g, '/') + '.cpp');
	compiler.addMacro('INANITY_LIB');
};

/**
 * структура проекта
 */
var libraries = {
	// ****** базовая библиотека
	'libinanity-base': {
		objects: [
		// совсем общее
		'Object', 'ManagedHeap', 'Strings', 'Exception',
		// синхронизация и потоки выполнения
		'Thread', 'CriticalSection', 'CriticalCode', 'Semaphore',
		// общее: файлы и потоки
		'File', 'EmptyFile', 'PartFile', 'MemoryFile', 'OutputStream', 'FileInputStream', 'StreamReader', 'StreamWriter', 'BufferedInputStream', 'BufferedOutputStream', 'MemoryStream',
		// преобразующие потоки
		'Base64OutputStream', 'Out2InStream',
		// файловые системы
		'FileSystem', 'FolderFileSystem', 'Handle', 'DiskInputStream', 'DiskOutputStream', 'BlobFileSystem']
	},
	// ******* сетевая библиотека
	'libinanity-net': {
		objects: [
		// базовая часть
		'EventLoop',
		// сокеты
		'Socket', 'ClientSocket', 'ServerSocket',
		// HTTP
		'HttpClient', 'HttpResponseStream', 'http_parser']
	},
	// ******* скрипты на lua
	'libinanity-lua': {
		objects: ['LuaState']
	},
	// ******* подсистема ввода
	'libinanity-input': {
		objects: ['input.Frame', 'input.Manager', 'input.Mux', 'input.Processor', 'input.RawManager']
	},
	// ******* общая графика
	'libinanity-graphics': {
		objects: ['graphics.Geometry', 'graphics.GeometryFormat', 'graphics.Window', 'graphics.RenderStage']
	},
	// ******* подсистема DX
	'libinanity-dx': {
		objects: ['graphics.dx.System', 'graphics.dx.Context', 'graphics.dx.RenderBuffer', 'graphics.dx.Texture', 'graphics.dx.DepthStencilBuffer']
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
