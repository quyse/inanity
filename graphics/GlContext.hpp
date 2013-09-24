#ifndef ___INANITY_GRAPHICS_GL_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_GL_CONTEXT_HPP___

#include "Context.hpp"
#include "opengl.hpp"
#include <map>

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlInternalProgramCache;
class GlInternalProgram;
class GlRenderBuffer;

/// Класс контекста OpenGL.
class GlContext : public Context
{
private:
	ptr<GlDevice> device;

	/// Current bound framebuffer.
	GLuint boundFrameBuffer;

	/// Количество используемых атрибутов.
	int boundAttributesCount;

	/// Кэш программ.
	ptr<GlInternalProgramCache> programCache;
	/// Текущая программа.
	ptr<GlInternalProgram> boundProgram;

	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

	/* Используется странное решение для очистки буферов глубины.
	Так как для очистки требуется полнота фреймбуфера, то есть
	хотя бы один цветовой буфер, мы привязываем бесполезный буфер. */
	/// Кэш пустых рендербуферов для очистки буфера глубины.
	std::map<std::pair<int, int>, ptr<GlRenderBuffer> > dummyRenderBuffers;
	/// Получить пустой рендербуфер нужных размеров.
	ptr<GlRenderBuffer> GetDummyRenderBuffer(int width, int height);

	void SetBufferData(GLenum target, GLuint bufferName, const void* data, int size, int bufferSize);

public:
	GlContext(ptr<GlDevice> device);

	// методы Context

	void ClearColor(int colorBufferIndex, const float* color);
	void ClearDepth(float depth);
	void ClearStencil(unsigned stencil);
	void ClearDepthStencil(float depth, unsigned stencil);

	void SetUniformBufferData(UniformBuffer* buffer, const void* data, int size);
	void SetVertexBufferData(VertexBuffer* buffer, const void* data, int size);

	void Draw(int count = -1);
	void DrawInstanced(int instancesCount, int count = -1);
};

END_INANITY_GRAPHICS

#endif
