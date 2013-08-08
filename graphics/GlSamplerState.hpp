#ifndef ___INANITY_GRAPHICS_GL_SAMPLER_STATE_HPP___
#define ___INANITY_GRAPHICS_GL_SAMPLER_STATE_HPP___

#include "SamplerState.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс настроек семплера OpenGL.
class GlSamplerState : public SamplerState
{
private:
	ptr<GlDevice> device;
	GLuint samplerName;

	/// Преобразовать режим свёртки в константу OpenGL.
	static GLint ConvertWrap(Wrap wrap);

	/// Обновить настройки семплера, если они менялись.
	void Update();

public:
	GlSamplerState(ptr<GlDevice> device, GLuint samplerName);
	~GlSamplerState();

	GLuint GetName();
};

END_INANITY_GRAPHICS

#endif
