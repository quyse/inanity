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
		'Object', 'ManagedHeap', 'Strings', 'StringTraveler', 'Exception',
		// время
		'Time',
		// профилирование
		'Profiling',
		// синхронизация и потоки выполнения
		'Thread', 'CriticalSection', 'CriticalCode', 'Semaphore',
		// общее: файлы и потоки
		'File', 'EmptyFile', 'PartFile', 'MemoryFile', 'OutputStream', 'FileInputStream', 'StreamReader', 'StreamWriter', 'BufferedInputStream', 'BufferedOutputStream', 'MemoryStream',
		// преобразующие потоки
		'Base64OutputStream', 'Out2InStream',
		// файловые системы
		'FileSystem', 'FolderFileSystem', 'FolderFile', 'Handle', 'DiskInputStream', 'DiskOutputStream', 'BlobFileSystem', 'BlobFileSystemBuilder', 'CompositeFileSystem', 'TempFileSystem',
		// ресурсы
		'ResourceManager', 'ResourceLoader']
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
	// ******* криптография
	'libinanity-crypto': {
		objects: ['crypto.HashStream', 'crypto.WhirlpoolStream']
	},
	// ******* подсистема ввода
	'libinanity-input': {
		objects: ['input.Frame', 'input.Manager', 'input.Mux', 'input.Processor', 'input.RawManager']
	},
	// ******* общая графика
	'libinanity-graphics': {
		objects: ['graphics.RenderStage', 'graphics.Window', 'graphics.GeometryFormat', 'graphics.EditableFont', 'graphics.Font', 'graphics.ShaderSource']
	},
	// ******* подсистема DX
	'libinanity-dx': {
		objects: [
			'graphics.dx.System', 'graphics.dx.Buffer', 'graphics.dx.Context', 'graphics.dx.RenderBuffer', 'graphics.dx.DepthStencilBuffer',
			'graphics.dx.DepthStencilState', 'graphics.dx.BlendState', 'graphics.dx.RasterizerState', 'graphics.dx.SamplerState',
			'graphics.dx.Geometry', 'graphics.dx.Texture',
			'graphics.dx.ShaderLayout', 'graphics.dx.ShaderReflection', 'graphics.dx.ConstantBuffer', 'graphics.dx.Shader', 'graphics.dx.VertexShader', 'graphics.dx.PixelShader',
			'graphics.dx.ShaderCompiler', 'graphics.dx.D3D10BlobFile', 'graphics.dx.FontDrawer']
	},
	// ******* подсистема GUI
	'libinanity-gui': {
		objects: [
			'gui.Element', 'gui.ContainerElement', 'gui.FreeContainer', 'gui.Label', 'gui.Button', 'gui.TextBox']
	}
};

var executables = {
	archi: {
		objects: ['archi.main', 'archi.BlobCreator', /*'archi.FontCreator',*/ /*'archi.SimpleGeometryCreator',*/ 'archi.SystemFontCreator'/*, 'archi.WavefrontObj', 'archi.XafConverter'*/],
		staticLibraries: ['libinanity-base', 'libinanity-graphics'],
		dynamicLibraries: ['user32.lib', 'gdi32.lib', 'comdlg32.lib']
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

exports.configureLinker = function(executableFile, linker) {
	// исполняемые файлы: <conf>/executable
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(executableFile);
	var confDir = a[1];
	linker.configuration = a[2];
	var executable = executables[a[3]];
	for ( var i = 0; i < executable.objects.length; ++i)
		linker.addObjectFile(confDir + executable.objects[i]);
	for ( var i = 0; i < executable.staticLibraries.length; ++i)
		linker.addStaticLibrary(confDir + executable.staticLibraries[i]);
	for ( var i = 0; i < executable.dynamicLibraries.length; ++i)
		linker.addDynamicLibrary(executable.dynamicLibraries[i]);
};
