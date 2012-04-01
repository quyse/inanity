#include "DepthStencilState.hpp"

String DepthStencilState::Cache::staticResourceName = "/inanity/dx/DepthStencilStateCache";

DepthStencilState::DepthStencilState(ptr<Cache> cache) : CachedState(cache)
{
	//установить настройки по умолчанию
	desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
}

void DepthStencilState::SetDepthSettings(bool enable, D3D11_DEPTH_WRITE_MASK writeMask, D3D11_COMPARISON_FUNC func)
{
	desc.DepthEnable = enable;
	desc.DepthWriteMask = writeMask;
	desc.DepthFunc = func;
	state = 0;
}

void DepthStencilState::SetStencilEnable(bool enable)
{
	desc.StencilEnable = enable;
	state = 0;
}

void DepthStencilState::SetStencilSettings(bool enable, UINT8 readMask, UINT8 writeMask, const D3D11_DEPTH_STENCILOP_DESC& frontFace, const D3D11_DEPTH_STENCILOP_DESC& backFace)
{
	desc.StencilEnable = enable;
	desc.StencilReadMask = readMask;
	desc.StencilWriteMask = writeMask;
	desc.FrontFace = frontFace;
	desc.BackFace = backFace;
	state = 0;
}
