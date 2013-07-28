#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_HPP___

#include "graphics.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс, инкапсулирующий слинкованную программу OpenGL.
/** Из программ составляется кэш. */
class GlInternalProgram : public Object
{
private:
	ptr<GlDevice> device;
	GLuint name;

public:
	GlInternalProgram(ptr<GlDevice> device, GLuint name);
	~GlInternalProgram();

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
