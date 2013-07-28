#ifndef ___INANITY_GRAPHICS_GL_DEVICE_HPP___
#define ___INANITY_GRAPHICS_GL_DEVICE_HPP___

#include "Device.hpp"
#include "DataType.hpp"
#include "opengl.hpp"
#include "../String.hpp"

#ifdef ___INANITY_LINUX
#include "../platform/platform.hpp"
#include "../platform/x11.hpp"
#endif

#ifdef ___INANITY_LINUX

BEGIN_INANITY_PLATFORM

class X11Display;

END_INANITY_PLATFORM

#endif

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

#ifdef ___INANITY_WINDOWS
	/// Имя графического устройства.
	String deviceName;
	/// Контекст рендеринга OpenGL.
	/** Создаётся при первом создании Presenter'а.
	Является общим для всех Presenter'ов. */
	HGLRC hglrc;
#endif
#ifdef ___INANITY_LINUX
	ptr<Platform::X11Display> display;
	/// Контекст рендеринга OpenGL.
	/** Создаётся при первом создании Presenter'а.
	Является общим для всех Presenter'ов. */
	GLXContext glxContext;
#endif

	/// Скомпилировать шейдер.
	static void CompileShader(GLuint shaderName, ptr<File> file, ptr<GlShaderBindings>& shaderBindings);
	static void GetAttributeSizeAndType(DataType dataType, GLint& size, GLenum& type, bool& integer);

public:

#ifdef ___INANITY_WINDOWS
	GlDevice(ptr<GlSystem> system, const String& deviceName);
#endif

#ifdef ___INANITY_LINUX
	GlDevice(ptr<GlSystem> system);
#endif

	~GlDevice();

	// методы Device
	ptr<System> GetSystem() const;
	ptr<Presenter> CreatePresenter(ptr<Output> output, ptr<MonitorMode> mode);
	ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat);
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource);
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	ptr<PixelShader> CreatePixelShader(ptr<File> file);
	ptr<UniformBuffer> CreateUniformBuffer(int size);
	ptr<VertexBuffer> CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout);
	ptr<VertexBuffer> CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout);
	ptr<IndexBuffer> CreateStaticIndexBuffer(ptr<File> file, int indexSize);
	ptr<AttributeBinding> CreateAttributeBinding(ptr<AttributeLayout> layout);
	ptr<Texture> CreateStaticTexture(ptr<RawTextureData> data);
	ptr<SamplerState> CreateSamplerState();
	ptr<BlendState> CreateBlendState();
};

END_INANITY_GRAPHICS

#endif
