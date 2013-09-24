#ifndef ___INANITY_GRAPHICS_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_HPP___

#include "ContextState.hpp"

BEGIN_INANITY_GRAPHICS

/// Abstract graphics context class.
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
	//******* Methods to clean color/depth/stencil buffers of bound framebuffer.
	/* These are immediate methods. */
	/// Clear color buffer.
	virtual void ClearColor(int colorBufferIndex, const float* color) = 0;
	/// Clear depth in depth-stencil buffer.
	virtual void ClearDepth(float depth) = 0;
	/// Clear stencil in depth-stencil buffer.
	virtual void ClearStencil(unsigned stencil) = 0;
	/// Clear depth & stencil in depth-stencil buffer.
	virtual void ClearDepthStencil(float depth, unsigned stencil) = 0;

	/// Получить требуемое состояние контекста.
	/** Его можно менять, чтобы задать то, что требуется. */
	ContextState& GetTargetState();

	/// Установить флаг форсирования сброса состояния.
	void Reset();

	//******* Методы для передачи данных.
	/* Непосредственные методы. */
	/// Указать данные для uniform-буфера.
	/** Размер передаваемых данных должен совпадать с размером буфера. */
	virtual void SetUniformBufferData(UniformBuffer* buffer, const void* data, int size) = 0;
	/// Указать данные для динамического вершинного буфера.
	virtual void SetVertexBufferData(VertexBuffer* buffer, const void* data, int size) = 0;

	//******* Методы рисования.
	/* Непосредственные методы. Кроме того, могут выполнять ранее отложенные действия по привязке данных. */
	/// Выполнить рисование.
	virtual void Draw(int count = -1) = 0;
	/// Выполнить instanced-рисование.
	virtual void DrawInstanced(int instancesCount, int count = -1) = 0;
};

END_INANITY_GRAPHICS

#endif
