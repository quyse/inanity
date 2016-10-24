#ifndef ___INANITY_GL_RENDER_BUFFER_HPP___
#define ___INANITY_GL_RENDER_BUFFER_HPP___

#include "RenderBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlInternalTexture;
class GlTexture;

/// Класс рендербуфера OpenGL.
/** Рендербуфер может быть трёх видов:
1. Рендербуфер по умолчанию (из оконного фреймбуфера).
renderBufferName = 0
texture = 0
2. Рендербуфер без текстуры.
renderBufferName = <name>
texture = 0
3. Текстура OpenGL (с возможностью использования в качестве рендербуфера).
renderBufferName = 0
texture = <texture>
*/
class GlRenderBuffer : public RenderBuffer
{
private:
	/// Внутренний текстурный объект.
	ptr<GlInternalTexture> internalTexture;
	/// Текстура рендербуфера.
	ptr<GlTexture> texture;

public:
	GlRenderBuffer(ptr<GlInternalTexture> internalTexture, ptr<GlTexture> texture);

	GLuint GetName() const;

	ptr<Texture> GetTexture();
	float GetScreenTopTexcoord() const;
};

END_INANITY_GRAPHICS

#endif
