project(inanity)

include_directories(
	deps/bullet/repo/src
	deps/libogg/include
	deps/libvorbis/include
	deps/freetype/repo/include
	deps/harfbuzz/generated
)

add_subdirectory(deps deps)

add_library(inanity-base STATIC
	Object.cpp
	ManagedHeap.cpp
	Strings.cpp
	StringTraveler.cpp
	Exception.cpp
	MemoryPool.cpp
	ChunkPool.cpp
	PoolObject.cpp
	Time.cpp
	Ticker.cpp
	Log.cpp
	Profiling.cpp
	Thread.cpp
	CriticalSection.cpp
	CriticalCode.cpp
	Semaphore.cpp
	File.cpp
	EmptyFile.cpp
	PartFile.cpp
	MemoryFile.cpp
	InputStream.cpp
	OutputStream.cpp
	FileInputStream.cpp
	MemoryStream.cpp
	StreamReader.cpp
	StreamWriter.cpp
	FileSystem.cpp
)

add_library(inanity-data STATIC
	data/FilterFileSystem.cpp
	data/TempFileSystem.cpp
	data/CompositeFileSystem.cpp
	data/BlobFileSystem.cpp
	data/BlobFileSystemBuilder.cpp
	data/Base64OutputStream.cpp
	data/Out2InStream.cpp
	data/BufferedFileSystem.cpp
	data/BufferedInputStream.cpp
	data/BufferedOutputStream.cpp
)

add_library(inanity-deflate STATIC
	data/DeflateStream.cpp
	data/InflateStream.cpp
)

add_library(inanity-sqlite STATIC
	data/sqlite.cpp
)

add_library(inanity-sqlitefs STATIC
	data/SQLiteFileSystem.cpp
)

add_library(inanity-lua STATIC
	script/lua/Any.cpp
	script/lua/Function.cpp
	script/lua/MetaProvider.cpp
	script/lua/State.cpp
	script/lua/stuff.cpp
)

add_library(inanity-crypto STATIC
	crypto/HashStream.cpp
	crypto/LamportSignatureAlgorithm.cpp
	crypto/WhirlpoolStream.cpp
	crypto/StreamHasher.cpp
	crypto/StreamSigner.cpp
	# Windows only
	crypto/CngRandomAlgorithm.cpp
)

add_library(inanity-input STATIC
	input/Event.cpp
	input/Frame.cpp
	input/Manager.cpp
	input/Mux.cpp
	input/Processor.cpp
	# Windows only
	input/Win32Manager.cpp
	input/Win32WmManager.cpp
	input/Win32RawManager.cpp
)

add_library(inanity-platform STATIC
	platform/Window.cpp
	platform/Game.cpp
	# Windows only
	platform/Win32Handle.cpp
	platform/Win32FileSystem.cpp
	platform/Win32Window.cpp
	graphics/Win32Adapter.cpp
	graphics/Win32Monitor.cpp
	graphics/Win32MonitorMode.cpp
	platform/DllCache.cpp
)

add_library(inanity-graphics STATIC
	graphics/Device.cpp
	graphics/Context.cpp
	graphics/FrameBuffer.cpp
	graphics/UniformBuffer.cpp
	graphics/VertexBuffer.cpp
	graphics/IndexBuffer.cpp
	graphics/SamplerSettings.cpp
	graphics/BlendState.cpp
	graphics/VertexLayout.cpp
	graphics/VertexLayoutElement.cpp
	graphics/AttributeLayout.cpp
	graphics/AttributeLayoutElement.cpp
	graphics/AttributeLayoutSlot.cpp
	graphics/ShaderCache.cpp
	graphics/PixelFormat.cpp
	graphics/RawTextureData.cpp
	graphics/TextureManager.cpp
	graphics/BmpImage.cpp
	graphics/PngImageLoader.cpp
	graphics/TgaImageLoader.cpp
)

add_library(inanity-dx11 STATIC
	graphics/Dx11System.cpp
	graphics/Dx11Device.cpp
	graphics/Dx11Context.cpp
	graphics/Dx11Presenter.cpp
	graphics/Dx11SwapChainPresenter.cpp
	graphics/Dx11FrameBuffer.cpp
	graphics/Dx11RenderBuffer.cpp
	graphics/Dx11DepthStencilBuffer.cpp
	graphics/Dx11Texture.cpp
	graphics/Dx11UniformBuffer.cpp
	graphics/Dx11VertexBuffer.cpp
	graphics/Dx11IndexBuffer.cpp
	graphics/Dx11AttributeBinding.cpp
	graphics/Dx11VertexShader.cpp
	graphics/Dx11PixelShader.cpp
	graphics/Dx11CompiledShader.cpp
	graphics/Dx11ShaderResources.cpp
	graphics/Dx11SamplerState.cpp
	graphics/Dx11BlendState.cpp
	graphics/D3DBlobFile.cpp
	graphics/Dx11ShaderCompiler.cpp
	graphics/Hlsl11Source.cpp
	graphics/DxgiAdapter.cpp
	graphics/DxgiMonitor.cpp
	graphics/DxgiMonitorMode.cpp
)

add_library(inanity-gl STATIC
	graphics/GlSystem.cpp
	graphics/GlDevice.cpp
	graphics/GlContext.cpp
	graphics/GlFrameBuffer.cpp
	graphics/GlInternalTexture.cpp
	graphics/GlInternalProgram.cpp
	graphics/GlInternalProgramCache.cpp
	graphics/GlRenderBuffer.cpp
	graphics/GlDepthStencilBuffer.cpp
	graphics/GlTexture.cpp
	graphics/GlUniformBuffer.cpp
	graphics/GlVertexBuffer.cpp
	graphics/GlIndexBuffer.cpp
	graphics/GlAttributeBinding.cpp
	graphics/GlVertexShader.cpp
	graphics/GlPixelShader.cpp
	graphics/GlSamplerState.cpp
	graphics/GlBlendState.cpp
	graphics/GlShaderCompiler.cpp
	graphics/GlslSource.cpp
	graphics/GlShaderBindings.cpp
	# Windows only
	graphics/WglPresenter.cpp
)

add_library(inanity-shaders STATIC
	graphics/shaders/AttributeNode.cpp
	graphics/shaders/SamplerNode.cpp
	graphics/shaders/UniformNode.cpp
	graphics/shaders/UniformGroup.cpp
	graphics/shaders/SwizzleNode.cpp
	graphics/shaders/OperationNode.cpp
	graphics/shaders/SampleNode.cpp
	graphics/shaders/FloatConstNode.cpp
	graphics/shaders/IntConstNode.cpp
	graphics/shaders/SequenceNode.cpp
	graphics/shaders/TransformedNode.cpp
	graphics/shaders/TempNode.cpp
	graphics/shaders/FragmentNode.cpp
	graphics/shaders/CastNode.cpp
	graphics/shaders/Expression.cpp
	graphics/shaders/Sampler.cpp
	graphics/shaders/GlslGenerator.cpp
	graphics/shaders/GlslGeneratorInstance.cpp
	graphics/shaders/Instancer.cpp
	graphics/shaders/Hlsl11Generator.cpp
	graphics/shaders/Hlsl11GeneratorInstance.cpp
)

add_library(inanity-physics STATIC
	physics/Shape.cpp
	physics/RigidBody.cpp
	physics/Character.cpp
)

add_library(inanity-bullet STATIC
	physics/BtWorld.cpp
	physics/BtShape.cpp
	physics/BtRigidBody.cpp
	physics/BtCharacter.cpp
)

add_library(inanity-gui STATIC
	gui/Element.cpp
	gui/ContainerElement.cpp
	gui/FreeContainer.cpp
	gui/ContentContainer.cpp
	gui/Window.cpp
	gui/Label.cpp
	gui/Button.cpp
	gui/TextBox.cpp
	gui/Visualizer.cpp
	gui/Font.cpp
	gui/Canvas.cpp
	gui/FontGlyphs.cpp
	# software
	gui/SwCanvas.cpp
	gui/SwFontGlyphs.cpp
	# graphics
	gui/GrCanvas.cpp
	gui/GrFontGlyphs.cpp
	# freetype
	gui/FtEngine.cpp
	gui/FtFontFace.cpp
	# harfbuzz
	gui/HbFontShape.cpp
)
