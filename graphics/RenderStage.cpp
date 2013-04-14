#include "RenderStage.hpp"
#include <algorithm>

BEGIN_INANITY_GRAPHICS

RenderStage::RenderTarget::RenderTarget() : clear(false)
{
	std::fill(clearColor, clearColor + 4, 0.0f);
}

RenderStage::RenderStage() : clearDepth(false), clearStencil(false)
{
}

void RenderStage::ResetRenderTargets()
{
	renderTargets.clear();
}

void RenderStage::SetRenderTarget(size_t slot, ptr<Graphics::RenderBuffer> renderBuffer, bool clear, const float* color)
{
	if(slot >= renderTargets.size())
		renderTargets.resize(slot + 1);
	renderTargets[slot].renderBuffer = renderBuffer;
	renderTargets[slot].clear = clear;
	if(clear)
		std::copy(color, color + 4, renderTargets[slot].clearColor);
}

void RenderStage::SetDepthStencilBuffer(ptr<Graphics::DepthStencilBuffer> depthStencilBuffer)
{
	this->depthStencilBuffer = depthStencilBuffer;
}

void RenderStage::SetDepthClearing(bool clear, float depth)
{
	clearDepth = clear;
	clearDepthValue = depth;
}

void RenderStage::SetStencilClearing(bool clear, unsigned stencil)
{
	clearStencil = clear;
	clearStencilValue = stencil;
}

void RenderStage::Apply(Graphics::Context* context)
{
	for(size_t i = 0; i < renderTargets.size(); ++i)
		if(renderTargets[i].clear)
			context->ClearRenderBuffer(renderTargets[i].renderBuffer, renderTargets[i].clearColor);

	if(depthStencilBuffer)
		if(clearDepth)
			if(clearStencil)
				context->ClearDepthStencilBuffer(depthStencilBuffer, clearDepthValue, clearStencilValue);
			else
				context->ClearDepthStencilBuffer(depthStencilBuffer, clearDepthValue);
		else if(clearStencil)
			context->ClearDepthStencilBuffer(depthStencilBuffer, clearStencilValue);

	renderBuffers.resize(renderTargets.size());
	for(size_t i = 0; i < renderTargets.size(); ++i)
		renderBuffers[i] = renderTargets[i].renderBuffer;

	context->SetRenderTargets(renderBuffers, depthStencilBuffer);

	renderBuffers.clear();
}

END_INANITY_GRAPHICS
