#ifndef ___INANITY_GRAPHICS_DX_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX_DEPTH_STENCIL_BUFFER_HPP___

#include "DepthStencilBuffer.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxTexture;

/// Класс буфера глубины-трафарета для DirectX 11.
class DxDepthStencilBuffer : public DepthStencilBuffer
{
private:
	ComPointer<ID3D11DepthStencilView> depthStencilView;
	ptr<DxTexture> texture;

public:
	DxDepthStencilBuffer(ComPointer<ID3D11DepthStencilView> depthStencilView, ptr<DxTexture> texture = 0);

	// методы DepthStencilBuffer
	ptr<Texture> GetTexture();

	ID3D11DepthStencilView* GetDepthStencilViewInterface();
};

END_INANITY_GRAPHICS

#endif
