#ifndef ___INANITY_GRAPHICS_GL_DEVICE_HPP___
#define ___INANITY_GRAPHICS_GL_DEVICE_HPP___

#include "Device.hpp"
#include "opengl.hpp"
#include "../String.hpp"

BEGIN_INANITY_GRAPHICS

class GlSystem;
class GlContext;

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
	/// Основной графический контекст.
	ptr<GlContext> context;

	/// Скомпилировать шейдер.
	static void CompileShader(GLuint shaderName, ptr<File> file);

public:
#ifdef ___INANITY_WINDOWS
	GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context);
	~GlDevice();
#endif

	// методы Device
	ptr<System> GetSystem() const;
	ptr<Presenter> CreatePresenter(ptr<Output> output, const PresentMode& mode);
	ptr<Context> GetContext();
	ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat);
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource);
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	ptr<PixelShader> CreatePixelShader(ptr<File> file);
	ptr<UniformBuffer> CreateUniformBuffer(int size);
	ptr<VertexBuffer> CreateVertexBuffer(ptr<File> file, ptr<Layout> layout);
	ptr<IndexBuffer> CreateIndexBuffer(ptr<File> file, int indexSize);
	ptr<Texture> CreateStaticTexture(ptr<File> file);
	ptr<SamplerState> CreateSamplerState();
	ptr<BlendState> CreateBlendState();
};

END_INANITY_GRAPHICS

#endif
