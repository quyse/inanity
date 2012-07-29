#ifndef ___INANITY_GRAPHICS_GL_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_GL_SAMPLER_HPP___

#include "Sampler.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс семплера OpenGL.
class GlSampler : public Sampler
{
private:
	GLuint samplerName;

	/// Преобразовать режим свёртки в константу OpenGL.
	static GLint ConvertWrap(Wrap wrap);

public:
	GlSampler(GLuint samplerName);
	~GlSampler();

	GLuint GetName() const;

	/// Обновить настройки семплера, если они менялись.
	void Update();
};

END_INANITY_GRAPHICS

#endif
