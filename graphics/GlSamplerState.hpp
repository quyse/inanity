#ifndef ___INANITY_GRAPHICS_GL_SAMPLER_STATE_HPP___
#define ___INANITY_GRAPHICS_GL_SAMPLER_STATE_HPP___

#include "SamplerState.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// OpenGL sampler state class.
class GlSamplerState : public SamplerState
{
private:
	ptr<GlDevice> device;
	GLuint name;

public:
	GlSamplerState(ptr<GlDevice> device, GLuint samplerName);
	~GlSamplerState();

	GLuint GetName();
};

END_INANITY_GRAPHICS

#endif
