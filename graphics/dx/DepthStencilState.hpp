#ifndef ___INANITY_DX_DEPTH_STENCIL_STATE_HPP___
#define ___INANITY_DX_DEPTH_STENCIL_STATE_HPP___

#include "CachedState.hpp"

BEGIN_INANITY_DX

class DepthStencilState : public CachedState<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>
{
public:
	DepthStencilState(ptr<Cache> cache);

	void SetDepthSettings(bool enable, D3D11_DEPTH_WRITE_MASK writeMask, D3D11_COMPARISON_FUNC func);
	void SetStencilEnable(bool enable);
	void SetStencilSettings(bool enable, UINT8 readMask, UINT8 writeMask, const D3D11_DEPTH_STENCILOP_DESC& frontFace, const D3D11_DEPTH_STENCILOP_DESC& backFace);
};

typedef DepthStencilState::Cache DepthStencilStateCache;

END_INANITY_DX

#endif
