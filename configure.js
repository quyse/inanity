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
};

var libraries = {
	// ****** base
	'libinanity-base': {
		objects: [
		'Object', 'ManagedHeap', 'Strings', 'StringTraveler', 'Exception',
		'MemoryPool', 'ChunkPool', 'PoolObject',
		'Time', 'Ticker',
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
		objects: ['script.np.Any', 'script.np.State', 'script.np.wrappers', 'script.np.Namespace']
	},
	// ******* криптография
	'libinanity-crypto': {
		objects: ['crypto.HashStream', 'crypto.LamportSignatureAlgorithm', 'crypto.WhirlpoolStream', 'crypto.StreamHasher', 'crypto.StreamSigner'],
		'objects-win32': ['crypto.CngRandomAlgorithm']
	},
	// ******* подсистема ввода
	'libinanity-input': {
		objects: ['input.Event', 'input.Frame', 'input.Manager', 'input.Mux', 'input.Processor'],
		'objects-win32': ['input.Win32Manager', 'input.Win32WmManager', 'input.Win32RawManager'],
		'objects-linux': ['input.SdlManager'],
		'objects-freebsd': ['input.SdlManager'],
		'objects-emscripten': ['input.SdlManager']
	},
	// ******* general platform subsystem
	'libinanity-platform': {
		objects: ['platform.Window', 'platform.Game'],
		'objects-win32': [
			'platform.Win32Handle', 'platform.Win32FileSystem',
			'platform.Win32Window',
			'graphics.Win32Adapter', 'graphics.Win32Monitor', 'graphics.Win32MonitorMode',
			'platform.DllCache'
		],
		'objects-linux': [
			'platform.PosixFileSystem',
			'platform.Sdl', 'platform.SdlWindow',
			'graphics.SdlAdapter', 'graphics.SdlMonitor', 'graphics.SdlMonitorMode'
		],
		'objects-freebsd': [
			'platform.PosixFileSystem',
			'platform.Sdl', 'platform.SdlWindow',
			'graphics.SdlAdapter', 'graphics.SdlMonitor', 'graphics.SdlMonitorMode'
		],
		'objects-emscripten': [
			'platform.PosixFileSystem',
			'platform.Sdl', 'platform.EmsWindow',
			'graphics.EmsAdapter', 'graphics.EmsMonitor'
		]
	},
	// ******* общая графика
	'libinanity-graphics': {
		objects: [
			'graphics.Device', 'graphics.Context',
			'graphics.FrameBuffer',
			'graphics.UniformBuffer', 'graphics.VertexBuffer', 'graphics.IndexBuffer',
			'graphics.SamplerSettings', 'graphics.BlendState',
			'graphics.VertexLayout', 'graphics.VertexLayoutElement',
			'graphics.AttributeLayout', 'graphics.AttributeLayoutElement', 'graphics.AttributeLayoutSlot',
			'graphics.ShaderCache',
			'graphics.PixelFormat', 'graphics.RawTextureData', 'graphics.TextureManager',
			'graphics.EditableFont', 'graphics.Font', 'graphics.FontManager', 'graphics.TextDrawer',
			'graphics.BmpImage', 'graphics.PngImageLoader'
		]
	},
	// ******* подсистема DirectX 11
	'libinanity-dx11': {
		objects: [
			'graphics.Dx11System', 'graphics.Dx11Device', 'graphics.Dx11Context',
			'graphics.Dx11Presenter', 'graphics.Dx11SwapChainPresenter',
			'graphics.Dx11FrameBuffer',
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
			'graphics.GlFrameBuffer',
			'graphics.GlInternalTexture', 'graphics.GlInternalProgram', 'graphics.GlInternalProgramCache',
			'graphics.GlRenderBuffer', 'graphics.GlDepthStencilBuffer', 'graphics.GlTexture', 'graphics.GlUniformBuffer',
			'graphics.GlVertexBuffer', 'graphics.GlIndexBuffer', 'graphics.GlAttributeBinding',
			'graphics.GlVertexShader', 'graphics.GlPixelShader',
			'graphics.GlSamplerState', 'graphics.GlBlendState',
			'graphics.GlShaderCompiler', 'graphics.GlslSource', 'graphics.GlShaderBindings'
		],
		'objects-win32': ['graphics.WglPresenter'],
		'objects-linux': ['graphics.SdlPresenter'],
		'objects-freebsd': ['graphics.SdlPresenter'],
		'objects-emscripten': ['graphics.EmsPresenter']
	},
	// ******* подсистема шейдеров
	'libinanity-shaders': {
		objects: [
			'graphics.shaders.AttributeNode', 'graphics.shaders.SamplerNode',
			'graphics.shaders.UniformNode', 'graphics.shaders.UniformGroup',
			'graphics.shaders.SwizzleNode', 'graphics.shaders.OperationNode',
			'graphics.shaders.SampleNode', 'graphics.shaders.FloatConstNode', 'graphics.shaders.IntConstNode',
			'graphics.shaders.SequenceNode',
			'graphics.shaders.TransformedNode', 'graphics.shaders.TempNode',
			'graphics.shaders.FragmentNode',
			'graphics.shaders.CastNode',
			'graphics.shaders.Expression', 'graphics.shaders.Sampler',
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
		objects: ['platform.NpapiPlugin', 'platform.NpapiPluginInstance']
	},
	// ******* подсистема GUI
	'libinanity-gui': {
		objects: [
			'gui.Element', 'gui.ContainerElement', 'gui.FreeContainer', 'gui.ContentContainer',
			'gui.Window', 'gui.Label', 'gui.Button', 'gui.TextBox',
			'gui.Visualizer',
			'gui.Font', 'gui.Canvas',
			// software
			'gui.SwCanvas', 'gui.SwFontGlyphs',
			// graphics
			'gui.GrCanvas', 'gui.GrFontGlyphs',
			// freetype
			'gui.FtEngine', 'gui.FtFontFace',
			// harfbuzz
			'gui.HbFontShape'
		]
	}
};

var executables = {
	archi: {
		objects: ['archi.main', 'archi.BlobCreator', /*'archi.FontCreator',*/ /*'archi.SimpleGeometryCreator',*/
			'archi.SystemFontCreator', 'archi.WavefrontObj', /*'archi.XafConverter'*/ 'archi.SkeletonConverter',
			'archi.BoneAnimationConverter'],
		staticLibraries: ['libinanity-base', 'libinanity-data', 'libinanity-graphics', 'libinanity-platform'],
		dynamicLibraries: ['user32.lib', 'gdi32.lib', 'comdlg32.lib']
	}
	, adapterstest: {
		objects: ['graphics.adapters_test'],
		staticLibraries: [
			'libinanity-dx11',
			'libinanity-platform',
			'libinanity-gl',
			'libinanity-graphics',
			'libinanity-platform',
			'libinanity-shaders',
			'libinanity-input',
			'libinanity-base',
			'deps/glew//libglew'],
		dynamicLibraries: ['user32.lib', 'gdi32.lib', 'dxgi.lib', 'd3d11.lib', 'opengl32.lib'], // win32
		//dynamicLibraries: ['GL', 'SDL2'], // linux
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
		staticLibraries: ['libinanity-base', 'libinanity-platform', 'libinanity-lua', 'deps/lua//liblua'],
		dynamicLibraries: []
	}
	// TEST
	, v8test: {
		objects: ['script.v8.test'],
		staticLibraries: ['libinanity-base', 'libinanity-platform', 'libinanity-v8', 'deps/v8//libv8_base', 'deps/v8//libv8_snapshot'],
		dynamicLibraries: ['ws2_32.lib', 'winmm.lib']
	}
	// TEST
	, nettesttcp: {
		objects: ['net.test-chat-tcp'],
		staticLibraries: ['libinanity-base', 'libinanity-asio', 'libinanity-platform'],
		dynamicLibraries: []
	}
	// TEST
	, nettestudp: {
		objects: ['net.test-chat-udp'],
		staticLibraries: ['libinanity-base', 'libinanity-asio', 'libinanity-platform'],
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
			'libinanity-graphics',
			'libinanity-gui',
			'libinanity-platform',
			'deps/freetype//libfreetype',
			'deps/harfbuzz//libharfbuzz',
			'libinanity-compress',
			'libinanity-base',
			],
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
