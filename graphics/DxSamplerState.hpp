#ifndef ___INANITY_GRAPHICS_DX_SAMPLER_STATE_HPP___
#define ___INANITY_GRAPHICS_DX_SAMPLER_STATE_HPP___

#include "SamplerState.hpp"
#include "../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс семплера DirectX.
class DxSamplerState : public SamplerState
{
private:
	ComPointer<ID3D11SamplerState> samplerState;

	static D3D11_TEXTURE_ADDRESS_MODE ConvertWrap(Wrap wrap);

	/// Обновить настройки семплера, если они менялись.
	void Update(ID3D11Device* deviceInterface);

public:
	ID3D11SamplerState* GetSamplerStateInterface(ID3D11Device* deviceInterface);
};

END_INANITY_GRAPHICS

#endif
