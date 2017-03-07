#ifndef ___INANITY_GRAPHICS_DX11_DEPTH_STENCIL_STATE_HPP___
#define ___INANITY_GRAPHICS_DX11_DEPTH_STENCIL_STATE_HPP___

#include "DepthStencilState.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11DepthStencilState : public DepthStencilState
{
private:
	ComPointer<ID3D11DepthStencilState> depthStencilState;

	static D3D11_COMPARISON_FUNC ConvertTestFunc(TestFunc testFunc);
	static D3D11_STENCIL_OP ConvertStencilOp(StencilOp op);
	void Update(ID3D11Device* deviceInterface);

public:
	ID3D11DepthStencilState* GetDepthStencilStateInterface(ID3D11Device* deviceInterface);
};

END_INANITY_GRAPHICS

#endif
