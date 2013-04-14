#ifndef ___INANITY_GRAPHICS_DEVICE_HPP___
#define ___INANITY_GRAPHICS_DEVICE_HPP___

#include "graphics.hpp"
#include "PixelFormat.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class System;
class Presenter;
class Output;
struct PresentMode;
class Context;
class RenderBuffer;
class DepthStencilBuffer;
class VertexShader;
class PixelShader;
class UniformBuffer;
class Layout;
class VertexBuffer;
class IndexBuffer;
class Geometry;
class Texture;
class Image2DData;
class SamplerState;
class BlendState;

/// Абстрактный класс устройства вывода.
/** Основной класс графической подсистемы. Обычно олицетворяет видеокарту.
Содержит методы для создания ресурсов устройства вывода, и выдаёт контекст
рисования. */
class Device : public Object
{
public:
	/// Получить графическую систему.
	virtual ptr<System> GetSystem() const = 0;

	/// Создать выходное устройство.
	/**
	\param output Область вывода для рисования графики.
	\param fullscreen Полноэкранный режим?
	\param width Ширина окна или горизонтальное разрешение экрана.
	\param height Высота окна или вертикальное разрешение экрана.
	*/
	virtual ptr<Presenter> CreatePresenter(ptr<Output> output, const PresentMode& mode) = 0;

	/// Получить графический контекст.
	virtual ptr<Context> GetContext() = 0;

	// ******* Методы для создания ресурсов устройства.

	/// Создать рендербуфер.
	/** Всегда доступен для чтения GPU. */
	virtual ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat) = 0;
	/// Создать depth-stencil буфер.
	virtual ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource) = 0;

	/// Создать вершинный шейдер из скомпилированного бинарного файла.
	virtual ptr<VertexShader> CreateVertexShader(ptr<File> file) = 0;
	/// Создать пиксельный шейдер из скомпилированного бинарного файла.
	virtual ptr<PixelShader> CreatePixelShader(ptr<File> file) = 0;

	/// Создать константный буфер.
	virtual ptr<UniformBuffer> CreateUniformBuffer(int size) = 0;
	/// Создать вершинный буфер.
	virtual ptr<VertexBuffer> CreateVertexBuffer(ptr<File> file, ptr<Layout> layout) = 0;
	/// Создать индексный буфер.
	/**
	\param indexSize Размер одного индекса - 2 или 4.
	*/
	virtual ptr<IndexBuffer> CreateIndexBuffer(ptr<File> file, int indexSize) = 0;

	/// Создать геометрию.
	virtual ptr<Geometry> CreateGeometry(ptr<VertexBuffer> vertexBuffer, ptr<IndexBuffer> indexBuffer) = 0;

	/// Создать статическую текстуру из графического файла.
	/** FIXME: Метод должен быть удалён. Для создания текстур нужно использовать
	общий метод, принимающий ImageData. */
	virtual ptr<Texture> CreateStaticTexture(ptr<File> file) = 0;
	/// Создать статическую 2D-текстуру из графических данных.
	virtual ptr<Texture> CreateStatic2DTexture(ptr<Image2DData> imageData) = 0;

	/// Создать объект настроек семплирования.
	virtual ptr<SamplerState> CreateSamplerState() = 0;

	/// Создать объект настроек смешивания.
	virtual ptr<BlendState> CreateBlendState() = 0;
};

END_INANITY_GRAPHICS

#endif
