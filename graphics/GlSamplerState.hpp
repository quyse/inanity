#ifndef ___INANITY_GRAPHICS_GL_SAMPLER_STATE_HPP___
#define ___INANITY_GRAPHICS_GL_SAMPLER_STATE_HPP___

#include "SamplerState.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// OpenGL sampler state class.
/** Could be real OpenGL sampler, or an emulation. */
class GlSamplerState : public SamplerState
{
private:
	ptr<GlDevice> device;
	/// Sampler name, or 0 in case of emulation.
	GLuint name;

	/// Convert abstracted wrap into OpenGL enum.
	static GLint ConvertWrap(Wrap wrap);

	/// Update sampler settings if they changed.
	void Update();

public:
	GlSamplerState(ptr<GlDevice> device, GLuint samplerName);
	~GlSamplerState();

	GLuint GetName();
};

END_INANITY_GRAPHICS

#endif
