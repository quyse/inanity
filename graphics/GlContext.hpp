#ifndef ___INANITY_GRAPHICS_GL_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_GL_CONTEXT_HPP___

#include "Context.hpp"
#include "opengl.hpp"
#include <map>

BEGIN_INANITY_GRAPHICS

class GlInternalProgramCache;
class GlInternalProgram;
class GlRenderBuffer;

/// Класс контекста OpenGL.
class GlContext : public Context
{
private:
	/// Фреймбуфер для установки нестандартных рендертаргетов (целевой фреймбуфер).
	GLuint targetFramebuffer;
	/// Текущий фреймбуфер.
	GLuint boundFramebuffer;

	/// Количество используемых атрибутов.
	int boundAttributesCount;

	/// Кэш программ.
	ptr<GlInternalProgramCache> programCache;
	/// Текущая программа.
	ptr<GlInternalProgram> boundProgram;

	void BindTargetsFramebuffer();
	void BindDefaultFramebuffer();
	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

	/* Используется странное решение для очистки буферов глубины.
	Так как для очистки требуется полнота фреймбуфера, то есть
	хотя бы один цветовой буфер, мы привязываем бесполезный буфер. */
	/// Кэш пустых рендербуферов для очистки буфера глубины.
	std::map<std::pair<int, int>, ptr<GlRenderBuffer> > dummyRenderBuffers;
	/// Получить пустой рендербуфер нужных размеров.
	ptr<GlRenderBuffer> GetDummyRenderBuffer(int width, int height);

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
