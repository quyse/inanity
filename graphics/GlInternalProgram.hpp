#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_HPP___

#include "graphics.hpp"
#include "opengl.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс, инкапсулирующий слинкованную программу OpenGL.
/** Из программ составляется кэш. */
class GlInternalProgram : public Object
{
private:
	GLuint name;
	std::vector<String> attributes;

public:
	GlInternalProgram(GLuint name);
	~GlInternalProgram();

	GLuint GetName() const;
	std::vector<String>& GetAttributes();
};

END_INANITY_GRAPHICS

#endif
