#ifndef ___INANITY_GRAPHICS_GL_DEVICE_HPP___
#define ___INANITY_GRAPHICS_GL_DEVICE_HPP___

#include "Device.hpp"
#include "DataType.hpp"
#include "LayoutDataType.hpp"
#include "SamplerSettings.hpp"
#include "opengl.hpp"
#include "../String.hpp"

#ifdef ___INANITY_PLATFORM_LINUX

#include "../platform/platform.hpp"
#include <SDL2/SDL_video.h>

#endif // ___INANITY_PLATFORM_LINUX



BEGIN_INANITY_GRAPHICS

class GlSystem;
class GlShaderBindings;

/// Класс графического устройства OpenGL.
/** Текущие ограничения:
- только один возможный Presenter.
*/
class GlDevice : public Device
{
private:
	/// Графическая система.
	ptr<GlSystem> system;

#if defined(___INANITY_PLATFORM_WINDOWS)
	/// Имя графического устройства.
	String deviceName;
	/// Контекст рендеринга OpenGL.
	/** Создаётся при первом создании Presenter'а.
	Является общим для всех Presenter'ов. */
	HGLRC hglrc;
#elif defined(___INANITY_PLATFORM_LINUX)

	SDL_GLContext glContext;

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
			// ARB_vertex_attrib_binding, core since OpenGL 4.3
			vertexAttribBinding = 4,
			// ARB_framebuffer_object, core since OpenGL 3.0
			frameBufferObject = 8
		};
	};

private:
	int internalCaps;

	/// Init capabilities.
	void InitCaps();

	/// Скомпилировать шейдер.
	static void CompileShader(GLuint shaderName, ptr<File> file, ptr<GlShaderBindings>& shaderBindings);
	static void GetAttributeSizeAndType(DataType dataType, LayoutDataType layoutDataType, GLint& size, GLenum& type, bool& integer);

	static GLint ConvertSamplerSettingsWrap(SamplerSettings::Wrap wrap);
	static void ConvertSamplerSettingsFilters(const SamplerSettings& samplerSettings, GLint& minFilter, GLint& magFilter);

	static void SetupTextureSampling(GLenum target, const SamplerSettings& samplerSettings);

public:

#if defined(___INANITY_PLATFORM_WINDOWS)
	GlDevice(ptr<GlSystem> system, const String& deviceName);
#elif defined(___INANITY_PLATFORM_LINUX)
	GlDevice(ptr<GlSystem> system);
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	GlDevice(ptr<GlSystem> system);
#else
#error Unknown platform
#endif

	~GlDevice();

	int GetInternalCaps() const;

	// методы Device
	ptr<System> GetSystem() const;
	ptr<Presenter> CreatePresenter(ptr<Output> output, ptr<MonitorMode> mode);
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
	ptr<AttributeBinding> CreateAttributeBinding(ptr<AttributeLayout> layout);
	ptr<Texture> CreateStaticTexture(ptr<RawTextureData> data, const SamplerSettings& samplerSettings);
	ptr<SamplerState> CreateSamplerState(const SamplerSettings& samplerSettings);
	ptr<BlendState> CreateBlendState();
};

END_INANITY_GRAPHICS

#endif
