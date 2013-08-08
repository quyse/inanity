#include "BlendState.hpp"

BEGIN_INANITY_GRAPHICS

BlendState::BlendState() : enable(false),
	sourceColor(colorSourceOne), destColor(colorSourceZero), colorOperation(operationAdd),
	sourceAlpha(alphaSourceOne), destAlpha(alphaSourceZero), alphaOperation(operationAdd),
	dirty(true)
{}

void BlendState::SetColor(ColorSource sourceColor, ColorSource destColor, Operation colorOperation)
{
	dirty = true;
	enable = true;
	this->sourceColor = sourceColor;
	this->destColor = destColor;
	this->colorOperation = colorOperation;
}

void BlendState::SetAlpha(AlphaSource sourceAlpha, AlphaSource destAlpha, Operation alphaOperation)
{
	dirty = true;
	enable = true;
	this->sourceAlpha = sourceAlpha;
	this->destAlpha = destAlpha;
	this->alphaOperation = alphaOperation;
}

END_INANITY_GRAPHICS
