#ifndef ___INANITY_GRAPHICS_DX11_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX11_DEPTH_STENCIL_BUFFER_HPP___

#include "DepthStencilBuffer.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11Texture;

/// Класс буфера глубины-трафарета для DirectX 11.
class Dx11DepthStencilBuffer : public DepthStencilBuffer
{
private:
	ComPointer<ID3D11DepthStencilView> depthStencilView;
	ptr<Dx11Texture> texture;

public:
	Dx11DepthStencilBuffer(ComPointer<ID3D11DepthStencilView> depthStencilView, ptr<Dx11Texture> texture = 0);

	// методы DepthStencilBuffer
	ptr<Texture> GetTexture();

	ID3D11DepthStencilView* GetDepthStencilViewInterface();
};

END_INANITY_GRAPHICS

#endif
