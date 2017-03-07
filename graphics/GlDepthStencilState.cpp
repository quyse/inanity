#include "GlDepthStencilState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GLenum GlDepthStencilState::ConvertTestFunc(TestFunc testFunc)
{
	GLenum func;
	switch(testFunc)
	{
	case testFuncNever:
		func = GL_NEVER;
		break;
	case testFuncLess:
		func = GL_LESS;
		break;
	case testFuncLessOrEqual:
		func = GL_LEQUAL;
		break;
	case testFuncEqual:
		func = GL_EQUAL;
		break;
	case testFuncNonEqual:
		func = GL_NOTEQUAL;
		break;
	case testFuncGreaterOrEqual:
		func = GL_GEQUAL;
		break;
	case testFuncGreater:
		func = GL_GREATER;
		break;
	case testFuncAlways:
		func = GL_ALWAYS;
		break;
	default:
		THROW("Unknown test func");
	}
	return func;
}

GLenum GlDepthStencilState::ConvertStencilOp(StencilOp op)
{
	GLenum o;
	switch(op)
	{
	case stencilOpKeep:
		o = GL_KEEP;
		break;
	case stencilOpZero:
		o = GL_ZERO;
		break;
	case stencilOpReplace:
		o = GL_REPLACE;
		break;
	case stencilOpIncrementClamp:
		o = GL_INCR;
		break;
	case stencilOpDecrementClamp:
		o = GL_DECR;
		break;
	case stencilOpIncrementWrap:
		o = GL_INCR_WRAP;
		break;
	case stencilOpDecrementWrap:
		o = GL_DECR_WRAP;
		break;
	case stencilOpInvert:
		o = GL_INVERT;
		break;
	default:
		THROW("Unknown stencil op");
	}
	return o;
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
