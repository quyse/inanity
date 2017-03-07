#include "DepthStencilState.hpp"

BEGIN_INANITY_GRAPHICS

DepthStencilState::StencilFace::StencilFace(TestFunc testFunc, StencilOp failOp, StencilOp depthFailOp, StencilOp passOp) :
	testFunc(testFunc),
	failOp(failOp),
	depthFailOp(depthFailOp),
	passOp(passOp)
{}

DepthStencilState::DepthStencilState() :
	depthTestFunc(testFuncAlways),
	depthWrite(false),
	stencilEnable(false),
	stencilMask(0xFF),
	stencilRef(0),
	dirty(true)
{}

void DepthStencilState::SetDepthTest(TestFunc depthTestFunc, bool depthWrite)
{
	dirty = true;
	this->depthTestFunc = depthTestFunc;
	this->depthWrite = depthWrite;
}

void DepthStencilState::SetStencil(bool stencilEnable, uint8_t stencilMask, uint8_t stencilRef)
{
	dirty = true;
	this->stencilEnable = stencilEnable;
	this->stencilMask = stencilMask;
	this->stencilRef = stencilRef;
}

void DepthStencilState::SetStencilFrontFace(const StencilFace& stencilFrontFace)
{
	dirty = true;
	this->stencilFrontFace = stencilFrontFace;
}

void DepthStencilState::SetStencilBackFace(const StencilFace& stencilBackFace)
{
	dirty = true;
	this->stencilBackFace = stencilBackFace;
}

END_INANITY_GRAPHICS
