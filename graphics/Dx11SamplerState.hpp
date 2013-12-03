#ifndef ___INANITY_GRAPHICS_DX11_SAMPLER_STATE_HPP___
#define ___INANITY_GRAPHICS_DX11_SAMPLER_STATE_HPP___

#include "SamplerState.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс семплера DirectX.
class Dx11SamplerState : public SamplerState
{
private:
	ComPointer<ID3D11SamplerState> samplerState;

public:
	Dx11SamplerState(ComPointer<ID3D11SamplerState> samplerState);

	ID3D11SamplerState* GetSamplerStateInterface(ID3D11Device* deviceInterface);
};

END_INANITY_GRAPHICS

#endif
