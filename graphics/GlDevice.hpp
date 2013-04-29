#ifndef ___INANITY_GRAPHICS_GL_DEVICE_HPP___
#define ___INANITY_GRAPHICS_GL_DEVICE_HPP___

#include "Device.hpp"
#include "opengl.hpp"
#include "../String.hpp"

#ifdef ___INANITY_LINUX
#include "../x11.hpp"
#endif

BEGIN_INANITY_GRAPHICS

class GlSystem;
class GlContext;
class GlInternalAttributeBinding;
class GlInternalAttributeBindingCache;
class GlShaderBindings;
class Layout;

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
	/// Контекст рендеринга OpenGL.
	/** Создаётся при первом создании Presenter'а.
	Является общим для всех Presenter'ов. */
	GLXContext glxContext;
#endif
	/// Основной графический контекст.
	ptr<GlContext> context;

	/// Кэш привязок аттрибутов.
	ptr<GlInternalAttributeBindingCache> attributeBindingCache;

	/// Скомпилировать шейдер.
	static void CompileShader(GLuint shaderName, ptr<File> file, ptr<GlShaderBindings>& shaderBindings);

public:

#ifdef ___INANITY_WINDOWS
	GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context);
	~GlDevice();
#endif

#ifdef ___INANITY_LINUX
	GlDevice(ptr<GlSystem> system, ptr<GlContext> context);
#endif

	/// Создать привязку атрибутов.
	ptr<GlInternalAttributeBinding> CreateInternalAttributeBinding(Layout* vertexLayout);

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
	ptr<Geometry> CreateGeometry(ptr<VertexBuffer> vertexBuffer, ptr<IndexBuffer> indexBuffer);
	ptr<Texture> CreateStaticTexture(ptr<File> file);
	ptr<Texture> CreateStatic2DTexture(ptr<Image2DData> imageData);
	ptr<SamplerState> CreateSamplerState();
	ptr<BlendState> CreateBlendState();
};

END_INANITY_GRAPHICS

#endif
