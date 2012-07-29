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
class VertexShader;
class PixelShader;
class UniformBuffer;
class VertexBuffer;
class IndexBuffer;
class Texture;

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
	virtual ptr<RenderBuffer> CreateRenderBuffer(size_t width, size_t height, PixelFormat pixelFormat) = 0;

	/// Создать вершинный шейдер из скомпилированного бинарного файла.
	virtual ptr<VertexShader> CreateVertexShader(ptr<File> file) = 0;
	/// Создать пиксельный шейдер из скомпилированного бинарного файла.
	virtual ptr<PixelShader> CreatePixelShader(ptr<File> file) = 0;

	/// Создать константный буфер.
	virtual ptr<UniformBuffer> CreateUniformBuffer(size_t size) = 0;
	/// Создать вершинный буфер.
	virtual ptr<VertexBuffer> CreateVertexBuffer(ptr<File> file, size_t vertexStride) = 0;
	/// Создать индексный буфер.
	/**
	\param indexSize Размер одного индекса - 2 или 4.
	*/
	virtual ptr<IndexBuffer> CreateIndexBuffer(ptr<File> file, size_t indexSize) = 0;

	/// Создать статическую текстуру из графического файла.
	virtual ptr<Texture> CreateStaticTexture(ptr<File> file) = 0;
};

END_INANITY_GRAPHICS

#endif
