#ifndef ___INANITY_GRAPHICS_DEVICE_HPP___
#define ___INANITY_GRAPHICS_DEVICE_HPP___

#include "graphics.hpp"
#include "shaders/shaders.hpp"
#include "PixelFormat.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_SHADERS

class ShaderGenerator;

END_INANITY_SHADERS

BEGIN_INANITY_PLATFORM

class Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class System;
class Presenter;
class ShaderCompiler;
class MonitorMode;
struct PresentMode;
class FrameBuffer;
class RenderBuffer;
class DepthStencilBuffer;
class VertexShader;
class PixelShader;
class UniformBuffer;
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class AttributeLayout;
class AttributeBinding;
class Texture;
class RawTextureData;
class SamplerState;
struct SamplerSettings;
class BlendState;

/// Абстрактный класс устройства вывода.
/** Основной класс графической подсистемы. Обычно олицетворяет видеокарту.
Содержит методы для создания ресурсов устройства вывода, и выдаёт контекст
рисования. */
class Device : public Object
{
public:
	struct Caps
	{
		enum _
		{
			/// Multiple attribute slots allowed, divisors are supported.
			attributeInstancing = 1,
			drawInstancing = 2
		};
		int flags;
		int maxColorBuffersCount;

		Caps();
	};

protected:
	/// Capabilities of the device.
	/** Should be set by subclasses. */
	Caps caps;

public:
	/// Get caps of device.
	const Caps& GetCaps() const;

	/// Получить графическую систему.
	virtual ptr<System> GetSystem() const = 0;

	/// Create presenter for window.
	virtual ptr<Presenter> CreateWindowPresenter(ptr<Platform::Window> window, ptr<MonitorMode> mode) = 0;

	/// Создать компилятор шейдеров.
	virtual ptr<ShaderCompiler> CreateShaderCompiler() = 0;
	/// Создать генератор шейдеров.
	virtual ptr<Shaders::ShaderGenerator> CreateShaderGenerator() = 0;

	// ******* Методы для создания ресурсов устройства.

	/// Creates framebuffer.
	virtual ptr<FrameBuffer> CreateFrameBuffer() = 0;

	/// Создать рендербуфер.
	/** Всегда доступен для чтения GPU. */
	virtual ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat, const SamplerSettings& samplerSettings) = 0;
	/// Создать depth-stencil буфер.
	virtual ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource) = 0;

	/// Создать вершинный шейдер из скомпилированного бинарного файла.
	virtual ptr<VertexShader> CreateVertexShader(ptr<File> file) = 0;
	/// Создать пиксельный шейдер из скомпилированного бинарного файла.
	virtual ptr<PixelShader> CreatePixelShader(ptr<File> file) = 0;

	/// Создать константный буфер.
	virtual ptr<UniformBuffer> CreateUniformBuffer(int size) = 0;
	/// Создать статический вершинный буфер.
	virtual ptr<VertexBuffer> CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout) = 0;
	/// Создать динамический вершинный буфер.
	virtual ptr<VertexBuffer> CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout) = 0;
	/// Создать статический индексный буфер.
	/**
	\param indexSize Размер одного индекса - 2 или 4.
	*/
	virtual ptr<IndexBuffer> CreateStaticIndexBuffer(ptr<File> file, int indexSize) = 0;

	/// Создать привязку атрибутов.
	virtual ptr<AttributeBinding> CreateAttributeBinding(ptr<AttributeLayout> layout) = 0;

	/// Создать статическую текстуру из файла с сырыми данными.
	virtual ptr<Texture> CreateStaticTexture(ptr<RawTextureData> data, const SamplerSettings& samplerSettings) = 0;

	/// Create sampler state from sampler settings.
	virtual ptr<SamplerState> CreateSamplerState(const SamplerSettings& samplerSettings) = 0;

	/// Создать объект настроек смешивания.
	virtual ptr<BlendState> CreateBlendState() = 0;
};

END_INANITY_GRAPHICS

#endif
