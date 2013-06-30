#ifndef ___INANITY_GRAPHICS_DX11_RENDER_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX11_RENDER_BUFFER_HPP___

#include "RenderBuffer.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11Texture;

/// Класс рендербуфера DirectX 11.
class Dx11RenderBuffer : public RenderBuffer
{
private:
	ComPointer<ID3D11RenderTargetView> renderTargetView;
	ptr<Dx11Texture> texture;

public:
	Dx11RenderBuffer(ComPointer<ID3D11RenderTargetView> renderTargetView, ptr<Dx11Texture> texture = 0);

	// методы RenderBuffer
	ptr<Texture> GetTexture();

	ID3D11RenderTargetView* GetRenderTargetViewInterface() const;
};

END_INANITY_GRAPHICS

#endif
