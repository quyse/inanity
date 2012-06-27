#include "RasterizerState.hpp"

String RasterizerState::Cache::staticResourceName = "/inanity/dx/RasterizerStateCache";

RasterizerState::RasterizerState(ptr<Cache> cache) : CachedState(cache)
{
	//установить настройки по умолчанию
	desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
}

void RasterizerState::SetFillMode(D3D11_FILL_MODE fillMode)
{
	desc.FillMode = fillMode;
	state = 0;
}

void RasterizerState::SetCullMode(D3D11_CULL_MODE cullMode)
{
	desc.CullMode = cullMode;
	state = 0;
}

void RasterizerState::SetMultisampleEnable(bool multisampleEnable)
{
	desc.MultisampleEnable = multisampleEnable;
	state = 0;
}
