#ifndef ___INANITY_GL_RENDER_BUFFER_HPP___
#define ___INANITY_GL_RENDER_BUFFER_HPP___

#include "RenderBuffer.hpp"

BEGIN_INANITY_GRAPHICS

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
	/// Имя рендербуфера.
	GLuint name;
	/// Текстура рендербуфера.
	ptr<GlTexture> texture;

public:
	GlRenderBuffer(GLuint name, ptr<GlTexture> texture);
	~GlRenderBuffer();

	GLuint GetName() const;
	/// Получить рендербуфер как текстуру.
	ptr<Texture> GetTexture();
};

END_INANITY_GRAPHICS

#endif
