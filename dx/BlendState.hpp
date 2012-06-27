#ifndef ___INANITY_DX_BLEND_STATE_HPP___
#define ___INANITY_DX_BLEND_STATE_HPP___

#include "CachedState.hpp"

BEGIN_INANITY_DX

class BlendState : public CachedState<ID3D11BlendState, D3D11_BLEND_DESC>
{
public:
	BlendState(ptr<Cache> cache);
};

typedef BlendState::Cache BlendStateCache;

END_INANITY_DX

#endif
