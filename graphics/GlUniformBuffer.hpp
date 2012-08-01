#ifndef ___INANITY_GRAPHICS_GL_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_UNIFORM_BUFFER_HPP___

#include "UniformBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс константного буфера OpenGL.
class GlUniformBuffer : public UniformBuffer
{
private:
	GLuint name;

public:
	GlUniformBuffer(GLuint name, size_t size);
	~GlUniformBuffer();

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
