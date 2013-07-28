#ifndef ___INANITY_GRAPHICS_GL_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_UNIFORM_BUFFER_HPP___

#include "UniformBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс константного буфера OpenGL.
class GlUniformBuffer : public UniformBuffer
{
private:
	ptr<GlDevice> device;
	GLuint name;

public:
	GlUniformBuffer(ptr<GlDevice> device, GLuint name, size_t size);
	~GlUniformBuffer();

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
