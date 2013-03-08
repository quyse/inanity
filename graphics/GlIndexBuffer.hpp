#ifndef ___INANITY_GRAPHIGS_GL_INDEX_BUFFER_HPP___
#define ___INANITY_GRAPHIGS_GL_INDEX_BUFFER_HPP___

#include "IndexBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс индексного буфера OpenGL.
class GlIndexBuffer : public IndexBuffer
{
private:
	GLuint name;

public:
	GlIndexBuffer(GLuint name, int indicesCount, int indexSize);
	~GlIndexBuffer();

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
