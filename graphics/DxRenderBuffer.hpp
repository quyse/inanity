#ifndef ___INANITY_GRAPHICS_DX_RENDER_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX_RENDER_BUFFER_HPP___

#include "RenderBuffer.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxTexture;

/// Класс рендербуфера DirectX 11.
class DxRenderBuffer : public RenderBuffer
{
private:
	ComPointer<ID3D11RenderTargetView> renderTargetView;
	ptr<DxTexture> texture;

public:
	DxRenderBuffer(ComPointer<ID3D11RenderTargetView> renderTargetView, ptr<DxTexture> texture = 0);

	// методы RenderBuffer
	ptr<Texture> GetTexture();

	ID3D11RenderTargetView* GetRenderTargetViewInterface() const;
};

END_INANITY_GRAPHICS

#endif
