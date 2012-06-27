#ifndef ___INANITY_GRAPHICS_DX_RENDER_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX_RENDER_BUFFER_HPP___

#include "RenderBuffer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс рендербуфера DirectX 11.
class DxRenderBuffer : public RenderBuffer
{
private:
	ComPointer<ID3D11RenderTargetView> renderTargetView;

public:
	DxRenderBuffer(ID3D11RenderTargetView* renderTargetView);
};

END_INANITY_GRAPHICS

#endif
