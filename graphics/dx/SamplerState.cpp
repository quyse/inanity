#include "SamplerState.hpp"

String SamplerState::Cache::staticResourceName = "/inanity/dx/SamplerStateCache";

SamplerState::SamplerState(ptr<Cache> cache) : CachedState(cache)
{
	//установить настройки по умолчанию
	desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
}

void SamplerState::SetFilter(D3D11_FILTER filter)
{
	desc.Filter = filter;
	state = 0;
}

void SamplerState::SetAddressMode(D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV, D3D11_TEXTURE_ADDRESS_MODE addressW)
{
	desc.AddressU = addressU;
	desc.AddressV = addressV;
	desc.AddressW = addressW;
	state = 0;
}

void SamplerState::SetBorderColor(const D3DXVECTOR4& borderColor)
{
	std::copy((const float*)borderColor, (const float*)borderColor + 4, desc.BorderColor);
	state = 0;
}
