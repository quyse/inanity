#ifndef ___INANITY_GRAPHICS_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_HPP___

#include "ContextState.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс графического контекста.
/** Контекст выполняет собственно рисование.
Передаёт состояние контекста в графическое API,
обновляя только то, что изменилось, и только тогда,
когда нужно.
Контекст удерживает ссылки как на объекты, уже
установленные в конвейер, так и на объекты, которым
это предстоит.
*/
class Context : public Object
{
protected:
	/// Реальное состояние контекста.
	ContextState boundState;
	/// Требуемое состояние контекста.
	ContextState targetState;
	/// Флаг форсирования сброса состояния.
	/** На следующем update будет обновлено всё состояние. */
	bool forceReset;

protected:
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

	/// Получить актуальное состояние контекста.
	/** Может использоваться, чтобы сделать из него
	новое состояние с минимальным количеством изменений. */
	const ContextState& GetBoundState() const;
	/// Получить требуемое состояние контекста.
	/** Его можно менять, чтобы задать то, что требуется. */
	ContextState& GetTargetState();

	/// Установить флаг форсирования сброса состояния.
	void Reset();

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
