#include "RenderBuffer.hpp"

RenderBuffer::RenderBuffer(ID3D11RenderTargetView* renderTargetView) :
	renderTargetView(renderTargetView)
{
}

ID3D11RenderTargetView* RenderBuffer::GetRenderTargetView() const
{
	return renderTargetView;
}
