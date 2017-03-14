#include "Dx11DepthStencilState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

D3D11_COMPARISON_FUNC Dx11DepthStencilState::ConvertTestFunc(TestFunc testFunc)
{
	switch(testFunc)
	{
	case testFuncNever:
		return D3D11_COMPARISON_NEVER;
	case testFuncLess:
		return D3D11_COMPARISON_LESS;
	case testFuncLessOrEqual:
		return D3D11_COMPARISON_LESS_EQUAL;
	case testFuncEqual:
		return D3D11_COMPARISON_EQUAL;
	case testFuncNonEqual:
		return D3D11_COMPARISON_NOT_EQUAL;
	case testFuncGreaterOrEqual:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case testFuncGreater:
		return D3D11_COMPARISON_GREATER;
	case testFuncAlways:
		return D3D11_COMPARISON_ALWAYS;
	default:
		THROW("Unknown test func");
	}
}

D3D11_STENCIL_OP Dx11DepthStencilState::ConvertStencilOp(StencilOp op)
{
	switch(op)
	{
	case stencilOpKeep:
		return D3D11_STENCIL_OP_KEEP;
	case stencilOpZero:
		return D3D11_STENCIL_OP_ZERO;
	case stencilOpReplace:
		return D3D11_STENCIL_OP_REPLACE;
	case stencilOpIncrementClamp:
		return D3D11_STENCIL_OP_INCR_SAT;
	case stencilOpDecrementClamp:
		return D3D11_STENCIL_OP_DECR_SAT;
	case stencilOpIncrementWrap:
		return D3D11_STENCIL_OP_INCR;
	case stencilOpDecrementWrap:
		return D3D11_STENCIL_OP_DECR;
	case stencilOpInvert:
		return D3D11_STENCIL_OP_INVERT;
	default:
		THROW("Unknown stencil op");
	}
}

void Dx11DepthStencilState::Update(ID3D11Device* deviceInterface)
{
	if(!dirty)
		return;

	depthStencilState = nullptr;

	BEGIN_TRY();

	D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

	desc.DepthEnable = depthTestFunc != testFuncAlways || depthWrite;
	desc.DepthWriteMask = depthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = ConvertTestFunc(depthTestFunc);
	desc.StencilEnable = stencilEnable;
	desc.StencilReadMask = stencilMask;
	desc.StencilWriteMask = stencilMask;
	desc.FrontFace.StencilFailOp = ConvertStencilOp(stencilFrontFace.failOp);
	desc.FrontFace.StencilDepthFailOp = ConvertStencilOp(stencilFrontFace.depthFailOp);
	desc.FrontFace.StencilPassOp = ConvertStencilOp(stencilFrontFace.passOp);
	desc.FrontFace.StencilFunc = ConvertTestFunc(stencilFrontFace.testFunc);
	desc.BackFace.StencilFailOp = ConvertStencilOp(stencilBackFace.failOp);
	desc.BackFace.StencilDepthFailOp = ConvertStencilOp(stencilBackFace.depthFailOp);
	desc.BackFace.StencilPassOp = ConvertStencilOp(stencilBackFace.passOp);
	desc.BackFace.StencilFunc = ConvertTestFunc(stencilBackFace.testFunc);

	if(FAILED(deviceInterface->CreateDepthStencilState(&desc, &depthStencilState)))
		THROW("Can't create depth-stencil state");

	dirty = false;

	END_TRY("Can't update DirectX depth-stencil state");
}

ID3D11DepthStencilState* Dx11DepthStencilState::GetDepthStencilStateInterface(ID3D11Device* deviceInterface)
{
	Update(deviceInterface);
	return depthStencilState;
}

END_INANITY_GRAPHICS
