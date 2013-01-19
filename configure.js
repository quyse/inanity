exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	// заменяем точки на слеши, и добавляем расширение
	// если оканчивается на .c - это .c-файл, иначе .cpp
	var source = a[2];
	var b = /^(.*)\.c$/.exec(source);
	if(b) {
		// .c-файл
		source = b[1].replace(/\./g, '/') + '.c';
		compiler.cppMode = false;
	}
	else
		// .cpp-файл
		source = source.replace(/\./g, '/') + '.cpp';
	compiler.setSourceFile(source);
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
		// очереди и обработчики
		'HandlerQueue',
		// общее: файлы и потоки
		'File', 'EmptyFile', 'PartFile', 'MemoryFile', 'InputStream', 'OutputStream', 'FileInputStream', 'StreamReader', 'StreamWriter', 'BufferedInputStream', 'BufferedOutputStream', 'MemoryStream',
		// преобразующие потоки
		'Base64OutputStream', 'Out2InStream',
		// файловые системы
		'FileSystem', 'FolderFileSystem', 'FolderFile', 'Handle', 'DiskInputStream', 'DiskOutputStream', 'BlobFileSystem', 'BlobFileSystemBuilder', 'CompositeFileSystem', 'TempFileSystem',
		// ресурсы
		'ResourceManager', 'ResourceLoader']
	},
	// ******* сжатие
	'libinanity-compress': {
		objects: [
		// зависимость - zlib
		'deps.zlib.adler32.c', 'deps.zlib.compress.c', 'deps.zlib.crc32.c', 'deps.zlib.deflate.c', /*'deps.zlib.gzclose.c', 'deps.zlib.gzlib.c', 'deps.zlib.gzread.c', 'deps.zlib.gzwrite.c',*/ 'deps.zlib.infback.c', 'deps.zlib.inflate.c', 'deps.zlib.inftrees.c', 'deps.zlib.inffast.c', 'deps.zlib.trees.c', 'deps.zlib.uncompr.c', 'deps.zlib.zutil.c',
		// потоки для сжатия
		'CompressStream', 'DecompressStream']
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
	// ******* файловая система на SQLite
	'libinanity-sqlitefs': {
		objects: ['SQLiteFileSystem', 'deps.sqlite.sqlite3.c']
	},
	// ******* скрипты на lua
	'libinanity-lua': {
		objects: ['lua.stuff', 'lua.reflection', 'lua.State', 'lua.Script']
	},
	// ******* криптография
	'libinanity-crypto': {
		objects: ['crypto.CngRandomAlgorithm', 'crypto.HashStream', 'crypto.LamportSignatureAlgorithm', 'crypto.WhirlpoolStream', 'crypto.StreamHasher', 'crypto.StreamSigner']
	},
	// ******* подсистема ввода
	'libinanity-input': {
		objects: ['input.Frame', 'input.Manager', 'input.Mux', 'input.Processor', 'input.RawManager']
	},
	// ******* подсистема окон
	'libinanity-win32window': {
		objects: ['Win32Window', 'graphics.Win32Output']
	},
	// ******* общая графика
	'libinanity-graphics': {
		objects: [
			'graphics.Context', 'graphics.ContextState',
			'graphics.UniformBuffer', 'graphics.VertexBuffer', 'graphics.Layout', 'graphics.LayoutBinding', 'graphics.IndexBuffer',
			'graphics.SamplerState', 'graphics.BlendState',
			'graphics.ShaderSource',
			'graphics.Geometry', 'graphics.Texture',
			'graphics.EditableFont']
	},
	// ******* подсистема DirectX 11
	'libinanity-dx': {
		objects: [
			'graphics.DxSystem', 'graphics.DxDevice', 'graphics.DxPresenter', 'graphics.DxContext',
			'graphics.DxInternalInputLayout', 'graphics.DxInternalInputLayoutCache',
			'graphics.DxRenderBuffer', 'graphics.DxDepthStencilBuffer', 'graphics.DxTexture', 'graphics.DxUniformBuffer',
			'graphics.DxVertexBuffer', 'graphics.DxIndexBuffer',
			'graphics.DxVertexShader', 'graphics.DxPixelShader',
			'graphics.DxSamplerState', 'graphics.DxBlendState',
			'graphics.D3D10BlobFile', 'graphics.DxShaderCompiler'
		]
	},
	// ******* подсистема OpenGl
	'libinanity-gl': {
		objects: [
			'graphics.GlSystem', 'graphics.GlDevice', 'graphics.GlPresenter', 'graphics.GlContext',
			'graphics.GlInternalTexture', 'graphics.GlInternalProgram', 'graphics.GlInternalProgramCache',
			'graphics.GlInternalAttributeBinding', 'graphics.GlInternalAttributeBindingCache',
			'graphics.GlRenderBuffer', 'graphics.GlDepthStencilBuffer', 'graphics.GlTexture', 'graphics.GlUniformBuffer',
			'graphics.GlVertexBuffer', 'graphics.GlIndexBuffer',
			'graphics.GlVertexShader', 'graphics.GlPixelShader'
		]
	},
	// ******* подсистема шейдеров
	'libinanity-shaders': {
		objects: [
			'graphics.shaders.AttributeNode', 'graphics.shaders.SamplerNode',
			'graphics.shaders.UniformNode', 'graphics.shaders.UniformGroup',
			'graphics.shaders.SwizzleNode', 'graphics.shaders.OperationNode',
			'graphics.shaders.SampleNode', 'graphics.shaders.FloatConstNode',
			'graphics.shaders.SequenceNode', 'graphics.shaders.TransitionalNode',
			'graphics.shaders.TempNode', 'graphics.shaders.CastNode',
			'graphics.shaders.SpecialNode',
			'graphics.shaders.Expression', 'graphics.shaders.Sampler',
			'graphics.shaders.HlslGenerator', 'graphics.shaders.HlslGeneratorInstance',
		]
	},
	// ******* общая физика
	'libinanity-physics': {
		objects: [
			'physics.Shape', 'physics.RigidBody', 'physics.Character'
		]
	},
	// ******* физика bullet
	'libinanity-bullet': {
		objects: [
			'physics.BtWorld', 'physics.BtShape', 'physics.BtRigidBody', 'physics.BtCharacter'
		]
	},
	// ******* подсистема GUI
	'libinanity-gui': {
		objects: [
			'gui.Element', 'gui.ContainerElement', 'gui.FreeContainer', 'gui.Label', 'gui.Button', 'gui.TextBox']
	}
};

var executables = {
	archi: {
		objects: ['archi.main', 'archi.BlobCreator', /*'archi.FontCreator',*/ /*'archi.SimpleGeometryCreator',*/
			'archi.SystemFontCreator', 'archi.WavefrontObj', /*'archi.XafConverter'*/ 'archi.SkeletonConverter',
			'archi.BoneAnimationConverter'],
		staticLibraries: ['libinanity-base', 'libinanity-graphics', 'libinanity-lua', 'deps/lua//liblua'],
		dynamicLibraries: ['user32.lib', 'gdi32.lib', 'comdlg32.lib']
	}
	// TEST
	, shaderstest: {
		objects: ['graphics.shaders.test'],
		staticLibraries: ['libinanity-base', 'libinanity-shaders'],
		dynamicLibraries: []
	}
	// TEST
	, graphicstest: {
		objects: ['graphics.test'],
		staticLibraries: ['libinanity-base', 'libinanity-dx'],
		dynamicLibraries: []
	}
	// TEST
	, luatest: {
		objects: ['lua.test'],
		staticLibraries: ['libinanity-base', 'libinanity-compress', 'libinanity-lua', 'deps/lua//liblua'],
		dynamicLibraries: []
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
	for ( var i = 0; i < executable.staticLibraries.length; ++i) {
		var staticLibrary = executable.staticLibraries[i];
		var confPos = staticLibrary.indexOf('//');
		if(confPos >= 0)
			staticLibrary = staticLibrary.replace('//', '/' + confDir);
		else
			staticLibrary = confDir + staticLibrary;
		linker.addStaticLibrary(staticLibrary);
	}
	for ( var i = 0; i < executable.dynamicLibraries.length; ++i)
		linker.addDynamicLibrary(executable.dynamicLibraries[i]);
};
