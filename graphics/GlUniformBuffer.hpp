#ifndef ___INANITY_GRAPHICS_GL_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_UNIFORM_BUFFER_HPP___

#include "UniformBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY

class MemoryFile;

END_INANITY

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// OpenGL uniform buffer class.
/** Could be a real uniform buffer, or an emulation
if uniform buffers are not supported by a device. */
class GlUniformBuffer : public UniformBuffer
{
private:
	ptr<GlDevice> device;
	/// Name in case of real uniform buffer, 0 otherwise.
	GLuint name;
	/// Memory buffer in case of emulation.
	ptr<MemoryFile> file;

public:
	GlUniformBuffer(ptr<GlDevice> device, GLuint name, size_t size);
	~GlUniformBuffer();

	GLuint GetName() const;
	ptr<MemoryFile> GetFile() const;
};

END_INANITY_GRAPHICS

#endif
