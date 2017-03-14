#include "GlDepthStencilState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GLenum GlDepthStencilState::ConvertTestFunc(TestFunc testFunc)
{
	switch(testFunc)
	{
	case testFuncNever:
		return GL_NEVER;
	case testFuncLess:
		return GL_LESS;
	case testFuncLessOrEqual:
		return GL_LEQUAL;
	case testFuncEqual:
		return GL_EQUAL;
	case testFuncNonEqual:
		return GL_NOTEQUAL;
	case testFuncGreaterOrEqual:
		return GL_GEQUAL;
	case testFuncGreater:
		return GL_GREATER;
	case testFuncAlways:
		return GL_ALWAYS;
	default:
		THROW("Unknown test func");
	}
}

GLenum GlDepthStencilState::ConvertStencilOp(StencilOp op)
{
	switch(op)
	{
	case stencilOpKeep:
		return GL_KEEP;
	case stencilOpZero:
		return GL_ZERO;
	case stencilOpReplace:
		return GL_REPLACE;
	case stencilOpIncrementClamp:
		return GL_INCR;
	case stencilOpDecrementClamp:
		return GL_DECR;
	case stencilOpIncrementWrap:
		return GL_INCR_WRAP;
	case stencilOpDecrementWrap:
		return GL_DECR_WRAP;
	case stencilOpInvert:
		return GL_INVERT;
	default:
		THROW("Unknown stencil op");
	}
}

void GlDepthStencilState::Apply()
{
	dirty = false;

	// depth test
	((depthTestFunc != testFuncAlways || depthWrite) ? glEnable : glDisable)(GL_DEPTH_TEST);
	glDepthFunc(ConvertTestFunc(depthTestFunc));
	glDepthMask(depthWrite ? GL_TRUE : GL_FALSE);

	// stencil test
	(stencilEnable ? glEnable : glDisable)(GL_STENCIL_TEST);
	if(stencilEnable)
	{
		glStencilFuncSeparate(GL_FRONT, stencilFrontFace.testFunc, stencilRef, stencilMask);
		glStencilOpSeparate(GL_FRONT, stencilFrontFace.failOp, stencilFrontFace.depthFailOp, stencilFrontFace.passOp);
		glStencilFuncSeparate(GL_BACK, stencilBackFace.testFunc, stencilRef, stencilMask);
		glStencilOpSeparate(GL_FRONT, stencilBackFace.failOp, stencilBackFace.depthFailOp, stencilBackFace.passOp);
	}
}

void GlDepthStencilState::ApplyDefault()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
}

END_INANITY_GRAPHICS
