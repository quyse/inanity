#ifndef ___INANITY_GRAPHICS_GL_DEPTH_STENCIL_STATE_HPP___
#define ___INANITY_GRAPHICS_GL_DEPTH_STENCIL_STATE_HPP___

#include "DepthStencilState.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDepthStencilState : public DepthStencilState
{
private:
	static GLenum ConvertTestFunc(TestFunc testFunc);
	static GLenum ConvertStencilOp(StencilOp op);

public:
	void Apply();
	static void ApplyDefault();
};

END_INANITY_GRAPHICS

#endif
