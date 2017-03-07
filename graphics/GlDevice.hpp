#ifndef ___INANITY_GRAPHICS_GL_DEVICE_HPP___
#define ___INANITY_GRAPHICS_GL_DEVICE_HPP___

#include "Device.hpp"
#include "DataType.hpp"
#include "LayoutDataType.hpp"
#include "SamplerSettings.hpp"
#include "opengl.hpp"
#include "../platform/platform.hpp"
#include "../String.hpp"

#if defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS)

#include <SDL2/SDL_video.h>

#endif // ___INANITY_PLATFORM_LINUX || ___INANITY_PLATFORM_FREEBSD || defined(___INANITY_PLATFORM_MACOS)



BEGIN_INANITY_PLATFORM

#if defined(___INANITY_PLATFORM_WINDOWS)
class Win32Window;
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS)
class SdlWindow;
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
class EmsWindow;
#else
#error Unknown platform
#endif

END_INANITY_PLATFORM



BEGIN_INANITY_GRAPHICS

class GlSystem;
class GlShaderBindings;

#if defined(___INANITY_PLATFORM_WINDOWS)
class WglPresenter;
class Win32MonitorMode;
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS)
class SdlPresenter;
class SdlMonitorMode;
class SdlWindow;
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
class EmsPresenter;
#else
#error Unknown platform
#endif



/// OpenGL graphics device.
class GlDevice : public Device
{
private:
	ptr<GlSystem> system;
	/// Currently bound presenter.
	/** Need for default framebuffers. */
	Presenter* boundPresenter;

#if defined(___INANITY_PLATFORM_WINDOWS)

	String deviceName;
	HGLRC hglrc;
	ptr<Platform::Win32Window> hiddenWindow;

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS)

	SDL_GLContext sdlContext;
	ptr<Platform::SdlWindow> hiddenWindow;

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	// nothing needed
#else
#error Unknown platform
#endif

public:
	//*** Supported internal capabilities.
	struct InternalCaps
	{
		enum _
		{
			// ARB_uniform_buffer_object, core since OpenGL 3.1
			uniformBufferObject = 1,
			// ARB_sampler_objects, core since OpenGL 3.3
			samplerObjects = 2,
			// ARB_vertex_array_object, core since OpenGL 3.0
			vertexArrayObject = 4,
			// ARB_vertex_attrib_binding, core since OpenGL 4.3
			vertexAttribBinding = 8,
			// ARB_framebuffer_object, core since OpenGL 3.0
			frameBufferObject = 16,
			// ARB_texture_storage, core since OpenGL 4.2
			textureStorage = 32
		};
	};

private:
	int internalCaps;

	/// Init capabilities.
	void InitCaps();

	static void CompileShader(GLuint shaderName, ptr<File> file, ptr<GlShaderBindings>& shaderBindings);
	static void GetAttributeSizeAndType(DataType dataType, LayoutDataType layoutDataType, GLint& size, GLenum& type, bool& integer);

	static GLint ConvertSamplerSettingsWrap(SamplerSettings::Wrap wrap);
	static void ConvertSamplerSettingsFilters(const SamplerSettings& samplerSettings, GLint& minFilter, GLint& magFilter);

	static void SetupTextureSampling(GLenum target, const SamplerSettings& samplerSettings);

public:

#if defined(___INANITY_PLATFORM_WINDOWS)
	GlDevice(ptr<GlSystem> system, const String& deviceName);
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS)
	GlDevice(ptr<GlSystem> system);
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	GlDevice(ptr<GlSystem> system);
#else
#error Unknown platform
#endif

	~GlDevice();

	int GetInternalCaps() const;

	/// Binds presenter if not already bound.
	void BindPresenter(Presenter* presenter);
	/// Unbind presenter if bound.
	void UnbindPresenter(Presenter* presenter);

	// Device's methods.
	ptr<System> GetSystem() const;
	ptr<Presenter> CreateWindowPresenter(ptr<Platform::Window> window, ptr<MonitorMode> mode);
	ptr<ShaderCompiler> CreateShaderCompiler();
	ptr<Shaders::ShaderGenerator> CreateShaderGenerator();
	ptr<FrameBuffer> CreateFrameBuffer();
	ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat, const SamplerSettings& samplerSettings);
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource);
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	ptr<PixelShader> CreatePixelShader(ptr<File> file);
	ptr<UniformBuffer> CreateUniformBuffer(int size);
	ptr<VertexBuffer> CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout);
	ptr<VertexBuffer> CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout);
	ptr<IndexBuffer> CreateStaticIndexBuffer(ptr<File> file, int indexSize);
	ptr<IndexBuffer> CreateDynamicIndexBuffer(int size, int indexSize);
	ptr<AttributeBinding> CreateAttributeBinding(ptr<AttributeLayout> layout);
	ptr<Texture> CreateStaticTexture(ptr<RawTextureData> data, const SamplerSettings& samplerSettings);
	ptr<SamplerState> CreateSamplerState(const SamplerSettings& samplerSettings);
	ptr<DepthStencilState> CreateDepthStencilState();
	ptr<BlendState> CreateBlendState();

	/// Create presenter routine.
#if defined(___INANITY_PLATFORM_WINDOWS)
	ptr<WglPresenter> CreatePresenter(ptr<Platform::Win32Window> window, ptr<Win32MonitorMode> mode);
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS)
	ptr<SdlPresenter> CreatePresenter(ptr<Platform::SdlWindow> window, ptr<SdlMonitorMode> mode);
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	ptr<EmsPresenter> CreatePresenter(ptr<Platform::EmsWindow> window, ptr<MonitorMode> mode);
#else
#error Unknown platform
#endif
};

END_INANITY_GRAPHICS

#endif
