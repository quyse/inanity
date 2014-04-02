#include "GlBlendState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlBlendState GlBlendState::defaultState;

GLenum GlBlendState::ConvertColorSource(ColorSource colorSource)
{
	GLenum cs;
	switch(colorSource)
	{
	case colorSourceZero: cs = GL_ZERO; break;
	case colorSourceOne: cs = GL_ONE; break;
	case colorSourceSrc: cs = GL_SRC_COLOR; break;
	case colorSourceInvSrc: cs = GL_ONE_MINUS_SRC_COLOR; break;
	case colorSourceSrcAlpha: cs = GL_SRC_ALPHA; break;
	case colorSourceInvSrcAlpha: cs = GL_ONE_MINUS_SRC_ALPHA; break;
	case colorSourceDest: cs = GL_DST_COLOR; break;
	case colorSourceInvDest: cs = GL_ONE_MINUS_DST_COLOR; break;
	case colorSourceDestAlpha: cs = GL_DST_ALPHA; break;
	case colorSourceInvDestAlpha: cs = GL_ONE_MINUS_DST_ALPHA; break;
	case colorSourceSecondSrc: cs = GL_SRC1_COLOR; break;
	case colorSourceInvSecondSrc: cs = GL_ONE_MINUS_SRC1_COLOR; break;
	case colorSourceSecondSrcAlpha: cs = GL_SRC1_ALPHA; break;
	case colorSourceInvSecondSrcAlpha: cs = GL_ONE_MINUS_SRC1_ALPHA; break;
	default:
		THROW("Unknown blend state color source");
	}
	return cs;
}

GLenum GlBlendState::ConvertAlphaSource(AlphaSource alphaSource)
{
	GLenum as;
	switch(alphaSource)
	{
	case alphaSourceZero: as = GL_ZERO; break;
	case alphaSourceOne: as = GL_ONE; break;
	case alphaSourceSrc: as = GL_SRC_ALPHA; break;
	case alphaSourceInvSrc: as = GL_ONE_MINUS_SRC_ALPHA; break;
	case alphaSourceDest: as = GL_DST_ALPHA; break;
	case alphaSourceInvDest: as = GL_ONE_MINUS_DST_ALPHA; break;
	case alphaSourceSecondSrc: as = GL_SRC1_ALPHA; break;
	case alphaSourceInvSecondSrc: as = GL_ONE_MINUS_SRC1_ALPHA; break;
	default:
		THROW("Unknown blend state alpha source");
	}
	return as;
}

GLenum GlBlendState::ConvertOperation(Operation operation)
{
	GLenum o;
	switch(operation)
	{
	case operationAdd: o = GL_FUNC_ADD; break;
	case operationSubtractAB: o = GL_FUNC_SUBTRACT; break;
	case operationSubtractBA: o = GL_FUNC_REVERSE_SUBTRACT; break;
	case operationMin: o = GL_MIN; break;
	case operationMax: o = GL_MAX; break;
	default:
		THROW("Unknown blend state operation");
	}
	return o;
}

void GlBlendState::Apply()
{
	// этому состоянию пофиг на грязность, поэтому всегда выставляем чистоту
	dirty = false;

	// установить всё
	(enable ? glEnable : glDisable)(GL_BLEND);
	glBlendFuncSeparate(ConvertColorSource(sourceColor), ConvertColorSource(destColor), ConvertAlphaSource(sourceAlpha), ConvertAlphaSource(destAlpha));
	glBlendEquationSeparate(ConvertOperation(colorOperation), ConvertOperation(alphaOperation));
}

void GlBlendState::ApplyDefault()
{
	defaultState.Apply();
}

END_INANITY_GRAPHICS
