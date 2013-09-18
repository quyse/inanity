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
	compiler.addIncludeDir('deps/bullet/src');
	compiler.addIncludeDir('deps/libogg/include');
	compiler.addIncludeDir('deps/libvorbis/include');
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
		'Time', 'Ticker',
		// синхронизация и потоки выполнения
		'Thread', 'CriticalSection', 'CriticalCode', 'Semaphore',
		// общее: файлы и потоки
		'EmptyFile', 'PartFile', 'MemoryFile', 'InputStream', 'OutputStream', 'FileInputStream', 'StreamReader', 'StreamWriter', 'BufferedInputStream', 'BufferedOutputStream', 'MemoryStream',
		// преобразующие потоки
		'Base64OutputStream', 'Out2InStream',
		// файловые системы
		'FileSystem', 'FolderFileSystem', 'FolderFile', 'Handle', 'DiskInputStream', 'DiskOutputStream',
		'BlobFileSystem', 'BlobFileSystemBuilder', 'CompositeFileSystem', 'TempFileSystem',
		'FilterFileSystem', 'BufferedFileSystem'
		]
	},
	// ******* profiling
	'libinanity-profile': {
		objects: [
		'profile.Profiler', 'profile.Reporter', 'profile.global'
		]
	},
	// ******* сжатие
	'libinanity-compress': {
		objects: [
		// зависимость - zlib
		'deps.zlib.adler32.c', 'deps.zlib.compress.c', 'deps.zlib.crc32.c', 'deps.zlib.deflate.c', /*'deps.zlib.gzclose.c', 'deps.zlib.gzlib.c', 'deps.zlib.gzread.c', 'deps.zlib.gzwrite.c',*/ 'deps.zlib.infback.c', 'deps.zlib.inflate.c', 'deps.zlib.inftrees.c', 'deps.zlib.inffast.c', 'deps.zlib.trees.c', 'deps.zlib.uncompr.c', 'deps.zlib.zutil.c',
		// потоки для сжатия
		'CompressStream', 'DecompressStream']
	},
	// ******* сетевая библиотека ASIO
	'libinanity-asio': {
		objects: [
		'net.asio', 'net.AsioService', 'net.AsioTcpListener', 'net.AsioTcpSocket', 'net.AsioInternalUdpSocket', 'net.AsioUdpListener', 'net.AsioUdpSocket']
	},
	// ******* библиотека HTTP
	'libinanity-http': {
		objects: [
		'net.HttpStream', 'net.HttpClient', 'deps.http-parser.http_parser.c']
	},
	// ******* файловая система на SQLite
	'libinanity-sqlitefs': {
		objects: ['SQLiteFileSystem']
	},
	// ******* метаинформация
	'libinanity-meta': {
		objects: ['meta.Class', 'meta.Function', 'meta.Method']
	},
	// ******* скрипты на lua
	'libinanity-lua': {
		objects: ['script.lua.Function', 'script.lua.State', 'script.lua.stuff']
	},
	// ******* скрипты на v8
	'libinanity-v8': {
		objects: ['script.v8.Function', 'script.v8.State']
	},
	// ******* криптография
	'libinanity-crypto': {
		objects: ['crypto.HashStream', 'crypto.LamportSignatureAlgorithm', 'crypto.WhirlpoolStream', 'crypto.StreamHasher', 'crypto.StreamSigner'],
		'objects-win32': ['crypto.CngRandomAlgorithm']
	},
	// ******* подсистема ввода
	'libinanity-input': {
		objects: ['input.Event', 'input.Frame', 'input.Manager', 'input.Mux', 'input.Processor'],
		'objects-win32': ['input.Win32Manager', 'input.Win32RawManager'],
		'objects-linux': ['input.X11Manager']
	},
	// ******* general platform subsystem
	'libinanity-platform': {
		objects: ['platform.Game'],
		'objects-win32': [
			'platform.Win32Window', 'graphics.Win32Output',
			'graphics.Win32Adapter', 'graphics.Win32Monitor', 'graphics.Win32MonitorMode',
			'platform.DllCache'
		],
		'objects-linux': ['platform.X11Window', 'platform.X11Display', 'graphics.X11Output', 'graphics.X11Adapter', 'graphics.X11Monitor', 'graphics.X11MonitorMode']
	},
	// ******* общая графика
	'libinanity-graphics': {
		objects: [
			'graphics.Context', 'graphics.ContextState', 'graphics.Output',
			'graphics.UniformBuffer', 'graphics.VertexBuffer', 'graphics.IndexBuffer',
			'graphics.SamplerState', 'graphics.BlendState',
			'graphics.VertexLayout', 'graphics.VertexLayoutElement',
			'graphics.AttributeLayout', 'graphics.AttributeLayoutElement', 'graphics.AttributeLayoutSlot',
			'graphics.ShaderCache',
			'graphics.PixelFormat', 'graphics.RawTextureData', 'graphics.TextureManager',
			'graphics.EditableFont', 'graphics.Font', 'graphics.FontManager', 'graphics.TextDrawer',
			'graphics.BmpImageLoader', 'graphics.PngImageLoader'
		]
	},
	// ******* подсистема DirectX 11
	'libinanity-dx11': {
		objects: [
			'graphics.Dx11System', 'graphics.Dx11Device', 'graphics.Dx11Presenter', 'graphics.Dx11Context',
			'graphics.Dx11RenderBuffer', 'graphics.Dx11DepthStencilBuffer', 'graphics.Dx11Texture', 'graphics.Dx11UniformBuffer',
			'graphics.Dx11VertexBuffer', 'graphics.Dx11IndexBuffer', 'graphics.Dx11AttributeBinding',
			'graphics.Dx11VertexShader', 'graphics.Dx11PixelShader',
			'graphics.Dx11CompiledShader', 'graphics.Dx11ShaderResources',
			'graphics.Dx11SamplerState', 'graphics.Dx11BlendState',
			'graphics.D3DBlobFile', 'graphics.Dx11ShaderCompiler', 'graphics.Hlsl11Source',
			'graphics.DxgiAdapter', 'graphics.DxgiMonitor', 'graphics.DxgiMonitorMode'
		]
	},
	// ******* подсистема OpenGl
	'libinanity-gl': {
		objects: [
			'graphics.GlSystem', 'graphics.GlDevice', 'graphics.GlContext',
			'graphics.GlInternalTexture', 'graphics.GlInternalProgram', 'graphics.GlInternalProgramCache',
			'graphics.GlRenderBuffer', 'graphics.GlDepthStencilBuffer', 'graphics.GlTexture', 'graphics.GlUniformBuffer',
			'graphics.GlVertexBuffer', 'graphics.GlIndexBuffer', 'graphics.GlAttributeBinding',
			'graphics.GlVertexShader', 'graphics.GlPixelShader',
			'graphics.GlSamplerState', 'graphics.GlBlendState',
			'graphics.GlShaderCompiler', 'graphics.GlslSource', 'graphics.GlShaderBindings'
		],
		'objects-win32': ['graphics.WglPresenter'],
		'objects-linux': ['graphics.GlxPresenter']
	},
	// ******* подсистема шейдеров
	'libinanity-shaders': {
		objects: [
			'graphics.shaders.AttributeNode', 'graphics.shaders.SamplerNode',
			'graphics.shaders.UniformNode', 'graphics.shaders.UniformGroup',
			'graphics.shaders.SwizzleNode', 'graphics.shaders.OperationNode',
			'graphics.shaders.SampleNode', 'graphics.shaders.FloatConstNode', 'graphics.shaders.IntConstNode',
			'graphics.shaders.SequenceNode',
			'graphics.shaders.TransformedNode', 'graphics.shaders.RasterizedNode',
			'graphics.shaders.TempNode', 'graphics.shaders.CastNode',
			'graphics.shaders.Expression', 'graphics.shaders.Sampler',
			'graphics.shaders.GlslGenerator', 'graphics.shaders.GlslGeneratorInstance'
		],
		'objects-win32': ['graphics.shaders.Hlsl11Generator', 'graphics.shaders.Hlsl11GeneratorInstance']
	},
	// ******* общее аудио
	'libinanity-audio': {
		objects: [
			'audio.Sound', 'audio.Source', 'audio.OggVorbisSource', 'audio.OggVorbisStream'
		]
	},
	// ******* OpenAL
	'libinanity-al': {
		objects: [
			'audio.AlSystem', 'audio.AlDevice', 'audio.AlBuffer', 'audio.AlPlayer',
			'audio.AlBufferedSound', 'audio.AlBufferedPlayer',
			'audio.AlStreamedSound', 'audio.AlStreamedPlayer'
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
		staticLibraries: ['libinanity-base', 'libinanity-graphics'],
		dynamicLibraries: ['user32.lib', 'gdi32.lib', 'comdlg32.lib']
	}
	, adapterstest: {
		objects: ['graphics.adapters_test'],
		staticLibraries: ['libinanity-base', 'libinanity-graphics', 'libinanity-shaders', 'libinanity-dx11', 'libinanity-gl', 'deps/glew//libglew', 'libinanity-platform'],
		dynamicLibraries: ['user32.lib', 'gdi32.lib', 'dxgi.lib', 'd3d11.lib', 'd3dx11.lib', 'd3dx10.lib', 'opengl32.lib']
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
	, audiotest: {
		objects: ['audio.test'],
		staticLibraries: ['libinanity-audio', 'libinanity-al', 'libinanity-base', 'deps/libvorbis//libvorbisfile', 'deps/libvorbis//libvorbis', 'deps/libogg//libogg'],
		dynamicLibraries: ['openal']
	}
	// TEST
	, luatest: {
		objects: ['script.lua.test'],
		staticLibraries: ['libinanity-base', 'libinanity-compress', 'libinanity-meta', 'libinanity-lua', 'deps/lua//liblua'],
		dynamicLibraries: []
	}
	// TEST
	, v8test: {
		objects: ['script.v8.test'],
		staticLibraries: ['libinanity-base', 'libinanity-meta', 'libinanity-v8', 'deps/v8//libv8_base', 'deps/v8//libv8_snapshot'],
		dynamicLibraries: ['ws2_32.lib', 'winmm.lib']
	}
	// TEST
	, nettesttcp: {
		objects: ['net.test-chat-tcp'],
		staticLibraries: ['libinanity-base', 'libinanity-asio'],
		dynamicLibraries: []
	}
	// TEST
	, nettestudp: {
		objects: ['net.test-chat-udp'],
		staticLibraries: ['libinanity-base', 'libinanity-asio'],
		dynamicLibraries: []
	}
	// TEST
	, nettesthttpclient: {
		objects: ['net.test-http-client'],
		staticLibraries: ['libinanity-base', 'libinanity-asio', 'libinanity-http'],
		dynamicLibraries: []
	}
	, profilingtest: {
		objects: ['test.profiling'],
		staticLibraries: ['libinanity-base'],
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
	var platformObjects = library['objects-' + composer.platform] || [];
	for(var i = 0; i < platformObjects.length; ++i)
		composer.addObjectFile(confDir + platformObjects[i]);
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
