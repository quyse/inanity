#include "Dx11SamplerState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

Dx11SamplerState::Dx11SamplerState(ComPointer<ID3D11SamplerState> samplerState)
: samplerState(samplerState) {}

ID3D11SamplerState* Dx11SamplerState::GetSamplerStateInterface(ID3D11Device* deviceInterface)
{
	return samplerState;
}

END_INANITY_GRAPHICS
