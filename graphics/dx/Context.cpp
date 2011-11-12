#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"

DX::Context::Context(ID3D11DeviceContext* context)
	: context(context), lastViewportWidth(0), lastViewportHeight(0)
{
}

ID3D11DeviceContext* DX::Context::GetDeviceContext() const
{
	return context;
}

void DX::Context::SetViewport(size_t width, size_t height)
{
	if(width == lastViewportWidth && height == lastViewportHeight) return;

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	context->RSSetViewports(1, &viewport);

	lastViewportWidth = width;
	lastViewportHeight = height;
}

void DX::Context::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	context->ClearRenderTargetView(renderBuffer->GetRenderTargetView(), color);
}

void DX::Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth)
{
	context->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH, depth, 0);
}

void DX::Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil)
{
	context->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_STENCIL, 0, stencil);
}

void DX::Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil)
{
	context->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void DX::Context::SetRenderTargets(const std::vector<ptr<RenderBuffer> >& renderBuffers, ptr<DepthStencilBuffer> depthStencilBuffer)
{
	renderTargets.resize(renderBuffers.size());
	for(size_t i = 0; i < renderBuffers.size(); ++i)
		renderTargets[i] = renderBuffers[i] ? renderBuffers[i]->GetRenderTargetView() : 0;
	context->OMSetRenderTargets(renderTargets.size(), renderTargets.empty() ? 0 : &renderTargets[0], depthStencilBuffer ? depthStencilBuffer->GetDepthStencilView() : 0);
	renderTargets.clear();
}
