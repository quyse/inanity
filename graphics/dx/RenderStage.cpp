#include "RenderStage.hpp"
#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"
#include <algorithm>

RenderStage::RenderStage()
: clearDepth(false), clearStencil(false), renderTargetsCount(0)
{
}

void RenderStage::ResetRenderTargets()
{
	std::fill(renderTargets, renderTargets + maxRenderTargetsCount, (RenderBuffer*)0);
	renderTargetsCount = 0;
}

void RenderStage::SetRenderTarget(unsigned slot, ptr<RenderBuffer> renderBuffer)
{
	renderTargets[slot] = renderBuffer;
	clearTargets[slot] = false;
	renderTargetsCount = std::max(renderTargetsCount, slot + 1);
}

void RenderStage::SetRenderTargetClearing(unsigned slot, bool clear, const float* color)
{
	clearTargets[slot] = clear;
	if(color) std::copy(color, color + 4, clearTargetColors[slot]);
}

void RenderStage::SetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer)
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

void RenderStage::Apply(Context* context)
{
	for(unsigned i = 0; i < renderTargetsCount; ++i)
		if(clearTargets[i] && renderTargets[i])
			context->ClearRenderBuffer(renderTargets[i], clearTargetColors[i]);

	if(depthStencilBuffer)
		if(clearDepth)
			if(clearStencil)
				context->ClearDepthStencilBuffer(depthStencilBuffer, clearDepthValue, clearStencilValue);
			else
				context->ClearDepthStencilBuffer(depthStencilBuffer, clearDepthValue);
		else if(clearStencil)
			context->ClearDepthStencilBuffer(depthStencilBuffer, clearStencilValue);

	ID3D11RenderTargetView* renderTargetViews[maxRenderTargetsCount];
	for(unsigned i = 0; i < renderTargetsCount; ++i)
		renderTargetViews[i] = renderTargets[i] ? renderTargets[i]->GetRenderTargetView() : 0;
	ID3D11DepthStencilView* depthStencilView = depthStencilBuffer ? depthStencilBuffer->GetDepthStencilView() : 0;
	context->GetDeviceContext()->OMSetRenderTargets(renderTargetsCount, renderTargetsCount ? renderTargetViews : NULL, depthStencilView);
}
