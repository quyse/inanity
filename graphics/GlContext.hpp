#ifndef ___INANITY_GRAPHICS_GL_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_GL_CONTEXT_HPP___

#include "Context.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlInternalProgramCache;

/// Класс контекста OpenGL.
class GlContext : public Context
{
private:
	/// Фреймбуфер для установки нестандартных рендертаргетов.
	GLuint targetsFramebuffer;
	/// Фреймбуфер для вспомогательных операций.
	GLuint serviceFramebuffer;
	/// Текущий фреймбуфер.
	/** То есть тот, который должен быть. Если флажок грязности фреймбуфера
	установлен, то он может быть неправильным.
	В сущности, может быть равен только targetsFramebuffer или 0 (нет фреймбуфера).
	*/
	GLuint currentFramebuffer;
	/// Флажок "грязности" текущего фреймбуфера.
	/** Если он стоит, то буфер следует установить в currentFramebuffer. */
	bool dirtyCurrentFramebuffer;

	/// Кэш программ.
	ptr<GlInternalProgramCache> programCache;

	void BindServiceFramebuffer();
	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

public:
	GlContext();
	~GlContext();

	// методы Context

	void ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color);
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth);
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil);
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil);

	void SetUniformBufferData(UniformBuffer* buffer, const void* data, size_t size);

	void Draw();
	void DrawInstanced(int instancesCount);
};

END_INANITY_GRAPHICS

#endif
