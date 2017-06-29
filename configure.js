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
	compiler.addIncludeDir('deps/bullet/repo/src');
	compiler.addIncludeDir('deps/libogg/include');
	compiler.addIncludeDir('deps/libvorbis/include');
	compiler.addIncludeDir('deps/freetype/repo/include');
	compiler.addIncludeDir('deps/harfbuzz/generated');
	compiler.addIncludeDir('deps/icu/repo/source/common');
	compiler.addIncludeDir('deps/mono/include');
	compiler.addMacro('U_STATIC_IMPLEMENTATION');

	// hack for deterministic floats
	if(source == 'math/HardFloat.cpp')
		compiler.fastMath = false;
};

var libraries = {
	// ****** base
	'libinanity-base': {
		objects: [
		'Object', 'ManagedHeap', 'Strings', 'StringTraveler', 'Exception',
		'MemoryPool', 'ChunkPool', 'PoolObject',
		'Time', 'Ticker',
		'Log',
		'Profiling',
		'Thread', 'CriticalSection', 'CriticalCode', 'Semaphore',
		'File', 'EmptyFile', 'PartFile', 'MemoryFile',
		'InputStream', 'OutputStream', 'FileInputStream', 'MemoryStream',
		'StreamReader', 'StreamWriter',
		'FileSystem'
		]
	},
	// ******* data
	'libinanity-data': {
		objects: [
		'data.FilterFileSystem',
		'data.TempFileSystem',
		'data.CompositeFileSystem',
		'data.BlobFileSystem', 'data.BlobFileSystemBuilder',
		'data.Base64OutputStream', 'data.Out2InStream',
		'data.BufferedFileSystem', 'data.BufferedInputStream', 'data.BufferedOutputStream'
		]
	},
	// ******* deflate compression
	'libinanity-deflate': {
		objects: ['data.DeflateStream', 'data.InflateStream']
	},
	// ******* lz4 compression
	'libinanity-lz4': {
		objects: ['data.Lz4CompressStream', 'data.Lz4DecompressStream']
	},
	// ******* OIL
	'libinanity-oil': {
		objects: ['data.oil.KeyValueStore']
	},
	// ******* net
	'libinanity-net': {
		objects: [
		'net.TcpSocket']
	},
	// ******* сетевая библиотека ASIO
	'libinanity-asio': {
		objects: [
		'net.asio', 'net.AsioService', 'net.AsioTcpListener', 'net.AsioTcpSocket', 'net.AsioInternalUdpSocket', 'net.AsioUdpListener', 'net.AsioUdpSocket']
	},
	// ******* FCGI support
	'libinanity-fcgi': {
		objects: ['net.Fcgi']
	},
	// ******* библиотека HTTP
	'libinanity-http': {
		objects: [
		'net.HttpStream', 'net.HttpClient']
	},
	// ******* SQLite поддержка
	'libinanity-sqlite': {
		objects: ['data.sqlite']
	},
	// ******* файловая система на SQLite
	'libinanity-sqlitefs': {
		objects: ['data.SQLiteFileSystem']
	},
	// ******* скрипты на lua
	'libinanity-lua': {
		objects: ['script.lua.Any', 'script.lua.Function', 'script.lua.MetaProvider', 'script.lua.State', 'script.lua.stuff']
	},
	// ******* скрипты на v8
	'libinanity-v8': {
		objects: ['script.v8.Any', 'script.v8.Function', 'script.v8.MetaProvider', 'script.v8.State']
	},
	// ******* скрипты на NPAPI
	'libinanity-np': {
		objects: ['script.np.Any', 'script.np.ClassBase', 'script.np.MetaProvider', 'script.np.State', 'script.np.wrappers', 'script.np.Namespace']
	},
	// ******* scripts for Mono
	'libinanity-mono': {
		objects: [
			'script.mono.Any',
			'script.mono.Assembly',
			'script.mono.ClassBase',
			'script.mono.MetaProvider',
			'script.mono.State'
		]
	},
	// ******* assembly generation for Mono scripts
	'libinanity-mono-gen': {
		objects: [
			'script.mono.BaseAssemblyGenerator',
			'script.mono.interop'
		]
	},
	// ******* криптография
	'libinanity-crypto': {
		objects: ['crypto.HashStream', 'crypto.LamportSignatureAlgorithm', 'crypto.WhirlpoolStream', 'crypto.StreamHasher', 'crypto.StreamSigner'],
		'objects-win32': ['crypto.CngRandomAlgorithm']
	},
	// ******* подсистема ввода
	'libinanity-input': {
		objects: ['input.Controller', 'input.Event', 'input.Frame', 'input.Manager', 'input.Mux', 'input.Processor'],
		'objects-win32': ['input.Win32Manager', 'input.Win32WmManager', 'input.Win32RawManager'],
		'objects-linux': ['input.SdlManager'],
		'objects-freebsd': ['input.SdlManager'],
		'objects-darwin': ['input.SdlManager'],
		'objects-android': ['input.SdlManager'],
		'objects-emscripten': ['input.SdlManager']
	},
	// ******* filesystem platform
	'libinanity-platform-filesystem': {
		objects: [],
		'objects-win32': [
			'platform.Win32Handle', 'platform.Win32FileSystem'
		],
		'objects-linux': [
			'platform.PosixFileSystem'
		],
		'objects-freebsd': [
			'platform.PosixFileSystem'
		],
		'objects-darwin': [
			'platform.PosixFileSystem'
		],
		'objects-android': [
			'platform.PosixFileSystem'
		],
		'objects-emscripten': [
			'platform.PosixFileSystem'
		]
	},
	// ******* general platform subsystem
	'libinanity-platform': {
		objects: ['platform.Window', 'platform.Game'],
		'objects-win32': [
			'platform.Win32Window',
			'graphics.Win32Adapter', 'graphics.Win32Monitor', 'graphics.Win32MonitorMode',
			'platform.DllCache'
		],
		'objects-linux': [
			'platform.Sdl', 'platform.SdlWindow',
			'graphics.SdlAdapter', 'graphics.SdlMonitor', 'graphics.SdlMonitorMode'
		],
		'objects-freebsd': [
			'platform.Sdl', 'platform.SdlWindow',
			'graphics.SdlAdapter', 'graphics.SdlMonitor', 'graphics.SdlMonitorMode'
		],
		'objects-darwin': [
			'platform.Sdl', 'platform.SdlWindow',
			'graphics.SdlAdapter', 'graphics.SdlMonitor', 'graphics.SdlMonitorMode'
		],
		'objects-android': [
			'platform.Sdl', 'platform.SdlWindow',
			'graphics.SdlAdapter', 'graphics.SdlMonitor', 'graphics.SdlMonitorMode'
		],
		'objects-emscripten': [
			'platform.Sdl', 'platform.EmsWindow',
			'graphics.EmsAdapter', 'graphics.EmsMonitor'
		]
	},
	// ******* raw graphics (no support for graphics API needed)
	'libinanity-graphics-raw': {
		objects: [
			'graphics.DataType',
			'graphics.VertexLayout', 'graphics.VertexLayoutElement',
			'graphics.PixelFormat', 'graphics.RawTextureData',
			'graphics.BmpImage', 'graphics.PngImageLoader', 'graphics.TgaImageLoader', 'graphics.UniversalImageLoader',
			'graphics.RawMesh'
		]
	},
	// ******* render graphics (need support for graphics API)
	'libinanity-graphics-render': {
		objects: [
			'graphics.Device', 'graphics.Context',
			'graphics.FrameBuffer',
			'graphics.UniformBuffer', 'graphics.VertexBuffer', 'graphics.IndexBuffer',
			'graphics.SamplerSettings', 'graphics.DepthStencilState', 'graphics.BlendState',
			'graphics.AttributeLayout', 'graphics.AttributeLayoutElement', 'graphics.AttributeLayoutSlot',
			'graphics.ShaderCache',
			'graphics.TextureManager',
			'graphics.Mesh'
		]
	},
	// ******* подсистема DirectX 11
	'libinanity-graphics-dx11': {
		objects: [
			'graphics.Dx11System', 'graphics.Dx11Device', 'graphics.Dx11Context',
			'graphics.Dx11Presenter', 'graphics.Dx11SwapChainPresenter',
			'graphics.Dx11FrameBuffer',
			'graphics.Dx11RenderBuffer', 'graphics.Dx11DepthStencilBuffer', 'graphics.Dx11Texture', 'graphics.Dx11UniformBuffer',
			'graphics.Dx11VertexBuffer', 'graphics.Dx11IndexBuffer', 'graphics.Dx11AttributeBinding',
			'graphics.Dx11VertexShader', 'graphics.Dx11PixelShader',
			'graphics.Dx11CompiledShader', 'graphics.Dx11ShaderResources',
			'graphics.Dx11SamplerState', 'graphics.Dx11DepthStencilState', 'graphics.Dx11BlendState',
			'graphics.D3DBlobFile', 'graphics.Dx11ShaderCompiler', 'graphics.Hlsl11Source',
			'graphics.DxgiAdapter', 'graphics.DxgiMonitor', 'graphics.DxgiMonitorMode'
		]
	},
	// ******* подсистема OpenGl
	'libinanity-graphics-gl': {
		objects: [
			'graphics.GlSystem', 'graphics.GlDevice', 'graphics.GlContext', 'graphics.GlPresenter',
			'graphics.GlFrameBuffer',
			'graphics.GlInternalTexture', 'graphics.GlInternalProgram', 'graphics.GlInternalProgramCache',
			'graphics.GlRenderBuffer', 'graphics.GlDepthStencilBuffer', 'graphics.GlTexture', 'graphics.GlUniformBuffer',
			'graphics.GlVertexBuffer', 'graphics.GlIndexBuffer', 'graphics.GlAttributeBinding',
			'graphics.GlVertexShader', 'graphics.GlPixelShader',
			'graphics.GlSamplerState', 'graphics.GlDepthStencilState', 'graphics.GlBlendState',
			'graphics.GlShaderCompiler', 'graphics.GlslSource', 'graphics.GlShaderBindings'
		],
		'objects-win32': ['graphics.WglPresenter'],
		'objects-linux': ['graphics.SdlPresenter'],
		'objects-freebsd': ['graphics.SdlPresenter'],
		'objects-darwin': ['graphics.SdlPresenter'],
		'objects-android': ['graphics.SdlPresenter'],
		'objects-emscripten': ['graphics.EmsPresenter']
	},
	// ******* подсистема шейдеров
	'libinanity-graphics-shaders': {
		objects: [
			'graphics.shaders.AttributeNode', 'graphics.shaders.SamplerNode',
			'graphics.shaders.UniformNode', 'graphics.shaders.ReadUniformNode', 'graphics.shaders.IndexUniformArrayNode',
			'graphics.shaders.SwizzleNode', 'graphics.shaders.OperationNode', 'graphics.shaders.ActionNode',
			'graphics.shaders.SampleNode', 'graphics.shaders.FloatConstNode', 'graphics.shaders.IntConstNode',
			'graphics.shaders.SequenceNode',
			'graphics.shaders.TransformedNode',
			'graphics.shaders.InterpolateNode', 'graphics.shaders.FragmentNode',
			'graphics.shaders.CastNode',
			'graphics.shaders.Expression',
			'graphics.shaders.UniformGroup',
			'graphics.shaders.Sampler',
			'graphics.shaders.SlGeneratorInstance',
			'graphics.shaders.GlslGenerator', 'graphics.shaders.GlslGeneratorInstance',
			'graphics.shaders.Instancer'
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
	// ******* NPAPI plugin
	'libinanity-npapi': {
		objects: [
			'platform.NpapiPlugin',
			'platform.NpapiPluginInstance',
			'platform.NpapiPresenter'
		],
	},
	// ******* подсистема GUI
	'libinanity-gui': {
		objects: [
			'gui.Element', 'gui.ContainerElement', 'gui.FreeContainer', 'gui.ContentContainer',
			'gui.Window', 'gui.Label', 'gui.Button', 'gui.TextBox',
			'gui.Visualizer',
			'gui.Font', 'gui.Canvas', 'gui.FontGlyphs',
			// software
			'gui.SwCanvas', 'gui.SwFontGlyphs',
			// graphics
			'gui.GrCanvas', 'gui.GrFontGlyphs',
			// freetype
			'gui.FtEngine', 'gui.FtFontFace',
			// harfbuzz
			'gui.HbFontShape'
		]
	},
	// ******* детерминированные числа
	'libinanity-hardfloat': {
		objects: [
			'math.HardFloat'
		]
	}
};

var executables = {
	archi: {
		objects: ['archi.main', 'archi.Vertex', 'archi.BlobCreator', /*'archi.FontCreator',*/ /*'archi.SimpleGeometryCreator',*/
			'archi.SystemFontCreator', 'archi.WavefrontObj', /*'archi.XafConverter'*/ 'archi.SkeletonConverter',
			'archi.BoneAnimationConverter', 'archi.AssimpConvertor'],
		staticLibraries: [
			'libinanity-data',
			'libinanity-graphics-raw',
			'libinanity-platform-filesystem',
			'deps/assimp//libassimp',
			'libinanity-base',
			'deps/zlib//libz'],
		'dynamicLibraries-win32': ['user32.lib', 'gdi32.lib', 'comdlg32.lib']
	}
	, adapterstest: {
		objects: ['graphics.adapters_test'],
		staticLibraries: [
			'libinanity-platform',
			'libinanity-gl',
			'libinanity-graphics',
			'libinanity-platform',
			'libinanity-shaders',
			'libinanity-input',
			'libinanity-base',
			'deps/glew//libglew'],
		'staticLibraries-win32': 'libinanity-dx11',
		'dynamicLibraries-win32': ['user32.lib', 'gdi32.lib', 'dxgi.lib', 'd3d11.lib', 'opengl32.lib'],
		'dynamicLibraries-linux': ['GL', 'SDL2'],
	}
	, oilfaucet: {
		objects: ['data.oil.faucet'],
		staticLibraries: [
			'libinanity-base',
			'libinanity-data',
			'libinanity-platform',
			'libinanity-lz4',
			'libinanity-sqlite',
			'libinanity-oil',
			'libinanity-crypto',
			'deps/sqlite//libsqlite',
			'deps/lz4//liblz4',
		],
		dynamicLibraries: []
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
		staticLibraries: ['libinanity-platform-filesystem', 'libinanity-lua', 'libinanity-base', 'deps/lua//liblua'],
		dynamicLibraries: []
	}
	// TEST
	, v8test: {
		objects: ['script.v8.test'],
		staticLibraries: ['libinanity-platform-filesystem', 'libinanity-v8', 'libinanity-base', 'deps/v8//libv8_base', 'deps/v8//libv8_snapshot'],
		'dynamicLibraries-win32': ['ws2_32.lib', 'winmm.lib'],
		'dynamicLibraries-linux': ['pthread'],
	}
	// TEST
	, monotest: {
		objects: ['script.mono.test'],
		staticLibraries: ['libinanity-mono', 'libinanity-mono-gen', 'libinanity-data', 'libinanity-platform-filesystem', 'libinanity-base'],
		dynamicLibraries: ['mono-2.0']
	}
	// TEST
	, nettesttcp: {
		objects: ['net.test-chat-tcp'],
		staticLibraries: ['libinanity-base', 'libinanity-asio', 'libinanity-platform-filesystem'],
		dynamicLibraries: []
	}
	// TEST
	, nettestudp: {
		objects: ['net.test-chat-udp'],
		staticLibraries: ['libinanity-base', 'libinanity-asio', 'libinanity-platform-filesystem'],
		dynamicLibraries: []
	}
	// TEST
	, nettesthttpclient: {
		objects: ['net.test-http-client'],
		staticLibraries: ['libinanity-base', 'libinanity-asio', 'libinanity-http'],
		dynamicLibraries: []
	}
	// TEST
	, testft: {
		objects: ['gui.testft'],
		staticLibraries: [
			'libinanity-graphics-raw',
			'libinanity-gui',
			'libinanity-platform-filesystem',
			'libinanity-deflate',
			'libinanity-base',
			'deps/harfbuzz//libharfbuzz',
			'deps/freetype//libfreetype',
			'deps/icu//libicu',
			'deps/ucdn//libucdn',
			'deps/zlib//libz'
			],
		dynamicLibraries: []
	}
};

var platformed = function(object, field, platform) {
	return (object[field] || []).concat(object[field + '-' + platform] || []);
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var library = libraries[a[3]];
	if(!library)
		throw "no library " + a[3];
	var objects = platformed(library, 'objects', composer.platform);
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};

exports.configureLinker = function(executableFile, linker) {
	// исполняемые файлы: <conf>/executable
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(executableFile);
	var confDir = a[1];
	linker.configuration = a[2];

	var executable = executables[a[3]];

	var objects = platformed(executable, 'objects', linker.platform);
	for ( var i = 0; i < objects.length; ++i)
		linker.addObjectFile(confDir + objects[i]);

	var staticLibraries = platformed(executable, 'staticLibraries', linker.platform);
	for ( var i = 0; i < staticLibraries.length; ++i) {
		var staticLibrary = staticLibraries[i];
		var confPos = staticLibrary.indexOf('//');
		if(confPos >= 0)
			staticLibrary = staticLibrary.replace('//', '/' + confDir);
		else
			staticLibrary = confDir + staticLibrary;
		linker.addStaticLibrary(staticLibrary);
	}

	var dynamicLibraries = platformed(executable, 'dynamicLibraries', linker.platform);
	for ( var i = 0; i < dynamicLibraries.length; ++i)
		linker.addDynamicLibrary(dynamicLibraries[i]);
};
