#ifndef ___INANITY_GRAPHIGS_GL_INDEX_BUFFER_HPP___
#define ___INANITY_GRAPHIGS_GL_INDEX_BUFFER_HPP___

#include "IndexBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс индексного буфера OpenGL.
class GlIndexBuffer : public IndexBuffer
{
private:
	ptr<GlDevice> device;
	GLuint name;

public:
	GlIndexBuffer(ptr<GlDevice> device, GLuint name, int indicesCount, int indexSize);
	~GlIndexBuffer();

	GLuint GetName() const;
	GLenum GetIndicesType() const;
};

END_INANITY_GRAPHICS

#endif
