#ifndef ___INANITY_GRAPHICS_GL_DEVICE_HPP___
#define ___INANITY_GRAPHICS_GL_DEVICE_HPP___

#include "Device.hpp"

BEGIN_INANITY_GRAPHICS

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

public:
#ifdef ___INANITY_WINDOWS
	GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context);
	~GlDevice();
#endif

	// методы Device
	ptr<System> GetSystem() const;
	ptr<Presenter> CreatePresenter(ptr<Output> output, const PresentMode& mode);
	ptr<Context> GetContext();
	ptr<RenderBuffer> CreateRenderBuffer(size_t width, size_t height, PixelFormat pixelFormat);
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	ptr<PixelShader> CreatePixelShader(ptr<File> file);
	ptr<ConstantBuffer> CreateConstantBuffer(size_t size);
	ptr<VertexBuffer> CreateVertexBuffer(ptr<File> file, size_t vertexStride);
	ptr<IndexBuffer> CreateIndexBuffer(ptr<File> file, size_t indexSize);
	ptr<Texture> CreateStaticTexture(ptr<File> file);
};

END_INANITY_GRAPHICS

#endif
