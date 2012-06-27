#ifndef ___INANITY_GRAPHICS_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс графического контекста.
/** Контекст управляет рисованием.
В контексте указываются различные настройки рендеринга,
задаются источники данных и так далее. Контекст должен
работать лениво, то есть передавать в своё API эти данные
только тогда, когда это действительно нужно (команда на
рисование). */
class Context : public Object
{
public:
	//******* Методы очистки.
	/// Очистить рендербуфер.
	virtual void ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color) = 0;
	/// Очистить глубину в depth-stencil буфере.
	virtual void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth) = 0;
	/// Очистить трафарет в depth-stencil буфере.
	virtual void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil) = 0;
	/// Очистить глубину и трафарет в depth-stencil буфере.
	virtual void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil) = 0;

	//******* Методы для привязки данных к конвейеру.
	/// Очистить все слоты вывода.
	virtual void ResetTargets() = 0;
	/// Указать рендербуфер в слот вывода.
	virtual void BindTargetRenderBuffer(size_t slot, ptr<RenderBuffer> renderBuffer) = 0;
	/// Указать depth-stencil буфер.
	virtual void BindTargetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer);
	/// Указать текстуру в текстурный слот.
	virtual void BindTexture(size_t slot, ptr<Texture> texture) = 0;
	/// Указать uniform-буфер в слот буферов.
	virtual void BindUniformBuffer(size_t slot, ptr<UniformBuffer> buffer) = 0;

	//******* Методы для передачи данных.
	/// Указать данные для uniform-буфера.
	/** Размер передаваемых данных должен совпадать с размером буфера. */
	virtual void SetUniformBufferData(UniformBuffer* buffer, const void* data, size_t size) = 0;
};

END_INANITY_GRAPHICS

#endif
