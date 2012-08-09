#ifndef ___INANITY_GRAPHICS_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class RenderBuffer;
class DepthStencilBuffer;
class Texture;
class Sampler;
class UniformBuffer;
class VertexShader;
class PixelShader;
class VertexBuffer;
class VertexLayout;
class IndexBuffer;

/// Абстрактный класс графического контекста.
/** Контекст управляет рисованием.
В контексте указываются различные настройки рендеринга,
задаются источники данных и так далее. Контекст должен
работать лениво, то есть передавать в своё API эти данные
только тогда, когда это действительно нужно (команда на
рисование).
*/
class Context : public Object
{
protected:
	/// Количество рендертаргетов.
	static const int renderTargetSlotsCount = 8;
	/// Привязанные рендертаргеты.
	ptr<RenderBuffer> boundRenderBuffers[renderTargetSlotsCount];
	bool dirtyRenderBuffers[renderTargetSlotsCount];
	/// Привязанный depth-stencil буфер.
	ptr<DepthStencilBuffer> boundDepthStencilBuffer;
	bool dirtyDepthStencilBuffer;

	/// Количество текстурных слотов.
	static const int textureSlotsCount = 16;
	/// Привязанные текстуры.
	ptr<Texture> boundTextures[textureSlotsCount];
	bool dirtyTextures[textureSlotsCount];
	/// Привязанные семплеры.
	/** Семплеры привязываются в той же нумерации, что и текстурные слоты. */
	ptr<Sampler> boundSamplers[textureSlotsCount];
	bool dirtySamplers[textureSlotsCount];

	/// Количество слотов для константных буферов.
	static const int uniformBufferSlotsCount = 16;
	/// Привязанные константные буферы.
	ptr<UniformBuffer> boundUniformBuffers[uniformBufferSlotsCount];
	bool dirtyUniformBuffers[uniformBufferSlotsCount];

	/// Привязанный вершинный шейдер.
	ptr<VertexShader> boundVertexShader;
	bool dirtyVertexShader;
	/// Привязанный пиксельный шейдер.
	ptr<PixelShader> boundPixelShader;
	bool dirtyPixelShader;

	/// Привязанный вершинный буфер.
	ptr<VertexBuffer> boundVertexBuffer;
	bool dirtyVertexBuffer;
	/// Привязанная вершинная разметка.
	ptr<VertexLayout> boundVertexLayout;
	bool dirtyVertexLayout;
	/// Привязанный индексный буфер.
	ptr<IndexBuffer> boundIndexBuffer;
	bool dirtyIndexBuffer;

	//******* Параметры растеризации.
	/// Режим заливки.
	enum FillMode
	{
		fillModeWireframe,
		fillModeSolid
	};
	FillMode fillMode;
	bool dirtyFillMode;
	/// Режим culling.
	enum CullMode
	{
		cullModeNone,
		cullModeBack,
		cullModeFront
	};
	CullMode cullMode;
	bool dirtyCullMode;

	/// Viewport.
	int viewportWidth, viewportHeight;
	bool dirtyViewport;

	/// Функция, сравнивающая глубину.
	/** Для отключения теста глубины нужно просто выставить эту функцию в always. */
	enum DepthTestFunc
	{
		depthTestFuncNever,
		depthTestFuncLess,
		depthTestFuncLessOrEqual,
		depthTestFuncEqual,
		depthTestFuncNonEqual,
		depthTestFuncGreaterOrEqual,
		depthTestFuncGreater,
		depthTestFuncAlways
	};
	DepthTestFunc depthTestFunc;
	bool dirtyDepthTestFunc;
	/// Запись в буфер глубины.
	bool depthWrite;
	bool dirtyDepthWrite;

protected:
	/// Выполнить инициализацию всех флажков и прочего.
	Context();

public:
	//******* Методы очистки рендербуферов и depth-stencil буферов.
	/* Непосредственные методы - тут же выполняют вызов к API. */
	/// Очистить рендербуфер.
	virtual void ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color) = 0;
	/// Очистить глубину в depth-stencil буфере.
	virtual void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth) = 0;
	/// Очистить трафарет в depth-stencil буфере.
	virtual void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil) = 0;
	/// Очистить глубину и трафарет в depth-stencil буфере.
	virtual void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil) = 0;

	//******* Методы для привязки данных к конвейеру.
	/* Работают лениво, то есть реальный вызов к API происходит, только когда нужно рисовать. */
	/// Очистить все слоты вывода.
	void ResetTargets();
	/// Указать рендербуфер в слот вывода.
	void BindTargetRenderBuffer(int slot, ptr<RenderBuffer> renderBuffer);
	/// Указать depth-stencil буфер.
	void BindTargetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer);
	/// Очистить все текстурные слоты.
	void ResetTextures();
	/// Указать текстуру в текстурный слот.
	void BindTexture(int slot, ptr<Texture> texture);
	/// Очистить все семплерные слоты.
	void ResetSamplers();
	/// Указать семплер в семплерный слот.
	void BindSampler(int slot, ptr<Sampler> sampler);
	/// Очистить все uniform-буферы.
	void ResetUniformBuffers();
	/// Указать uniform-буфер в слот буферов.
	void BindUniformBuffer(int slot, ptr<UniformBuffer> buffer);
	/// Указать вершинный шейдер.
	void BindVertexShader(ptr<VertexShader> vertexShader);
	/// Указать пиксельный шейдер.
	void BindPixelShader(ptr<PixelShader> pixelShader);
	/// Указать вершинный буфер.
	void BindVertexBuffer(ptr<VertexBuffer> vertexBuffer);
	/// Указать вершинную разметку.
	void BindVertexLayout(ptr<VertexLayout> vertexLayout);
	/// Указать индексный буфер.
	void BindIndexBuffer(ptr<IndexBuffer> indexBuffer);
	/// Указать режим заливки.
	void SetFillMode(FillMode fillMode);
	/// Указать режим culling.
	void SetCullMode(CullMode cullMode);
	/// Указать viewport.
	void SetViewport(int viewportWidth, int viewportHeight);
	/// Указать фукцию для теста глубины.
	void SetDepthTestFunc(DepthTestFunc depthTestFunc);
	/// Указать, включена ли запись глубины.
	void SetDepthWrite(bool depthWrite);

	//******* Методы для передачи данных.
	/* Непосредственные методы. */
	/// Указать данные для uniform-буфера.
	/** Размер передаваемых данных должен совпадать с размером буфера. */
	virtual void SetUniformBufferData(UniformBuffer* buffer, const void* data, size_t size) = 0;

	//******* Методы рисования.
	/* Непосредственные методы. Кроме того, могут выполнять ранее отложенные действия по привязке данных. */
	/// Выполнить рисование.
	/**
	\param indicesCount Количество индексов. -1, если нужно нарисовать весь индексный буфер.
	*/
	virtual void Draw() = 0;
	/// Выполнить instanced-рисование.
	virtual void DrawInstanced(int instancesCount) = 0;
};

END_INANITY_GRAPHICS

#endif
