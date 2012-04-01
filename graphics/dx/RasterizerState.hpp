#ifndef ___INANITY_DX_RASTERIZER_STATE_HPP___
#define ___INANITY_DX_RASTERIZER_STATE_HPP___

#include "CachedState.hpp"

BEGIN_INANITY_DX

class RasterizerState : public CachedState<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>
{
public:
	RasterizerState(ptr<Cache> cache);

	void SetFillMode(D3D11_FILL_MODE fillMode);
	void SetCullMode(D3D11_CULL_MODE cullMode);
	void SetMultisampleEnable(bool multisampleEnable);
};

typedef RasterizerState::Cache RasterizerStateCache;

END_INANITY_DX

#endif
