#ifndef ___INANITY_DX_SAMPLER_STATE_HPP___
#define ___INANITY_DX_SAMPLER_STATE_HPP___

#include "CachedState.hpp"
#include "d3dx.hpp"

BEGIN_INANITY_DX

class SamplerState : public CachedState<ID3D11SamplerState, D3D11_SAMPLER_DESC>
{
public:
	SamplerState(ptr<Cache> cache);

	void SetFilter(D3D11_FILTER filter);
	void SetAddressMode(
		D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE addressW = D3D11_TEXTURE_ADDRESS_CLAMP);
	void SetBorderColor(const D3DXVECTOR4& borderColor);
};

typedef SamplerState::Cache SamplerStateCache;

END_INANITY_DX

#endif
