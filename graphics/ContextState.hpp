#ifndef ___INANITY_GRAPHICS_CONTEXT_STATE_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_STATE_HPP___

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
class IndexBuffer;

/// Структура с состоянием контекста рисования.
/** В контексте указываются различные настройки рендеринга,
задаются источники данных и так далее. */
struct ContextState
{
	/// Количество рендертаргетов.
	static const int renderTargetSlotsCount = 8;
	/// Привязанные рендертаргеты.
	ptr<RenderBuffer> renderBuffers[renderTargetSlotsCount];
	/// Привязанный depth-stencil буфер.
	ptr<DepthStencilBuffer> depthStencilBuffer;

	/// Количество текстурных слотов.
	static const int textureSlotsCount = 16;
	/// Привязанные текстуры.
	ptr<Texture> textures[textureSlotsCount];
	/// Привязанные семплеры.
	/** Семплеры привязываются в той же нумерации, что и текстурные слоты. */
	ptr<Sampler> samplers[textureSlotsCount];

	/// Количество слотов для константных буферов.
	static const int uniformBufferSlotsCount = 16;
	/// Привязанные константные буферы.
	ptr<UniformBuffer> uniformBuffers[uniformBufferSlotsCount];

	/// Привязанный вершинный шейдер.
	ptr<VertexShader> vertexShader;
	/// Привязанный пиксельный шейдер.
	ptr<PixelShader> pixelShader;

	/// Привязанный вершинный буфер.
	ptr<VertexBuffer> vertexBuffer;
	/// Привязанный индексный буфер.
	ptr<IndexBuffer> indexBuffer;

	//******* Параметры растеризации.
	/// Режим заливки.
	enum FillMode
	{
		fillModeWireframe,
		fillModeSolid
	};
	FillMode fillMode;
	/// Режим culling.
	enum CullMode
	{
		cullModeNone,
		cullModeBack,
		cullModeFront
	};
	CullMode cullMode;

	/// Viewport.
	int viewportWidth, viewportHeight;

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
	/// Запись в буфер глубины.
	bool depthWrite;

	ContextState();

	/// Очистить все слоты вывода.
	void ResetTargets();
	/// Очистить все текстурные слоты.
	void ResetTextures();
	/// Очистить все семплерные слоты.
	void ResetSamplers();
	/// Очистить все uniform-буферы.
	void ResetUniformBuffers();
};

END_INANITY_GRAPHICS

#endif
