#ifndef ___INANITY_GRAPHICS_DX_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_DX_SAMPLER_HPP___

#include "Sampler.hpp"
#include "../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_GRAPHICS

class DxDevice;

/// Класс семплера DirectX.
class DxSampler : public Sampler
{
private:
	ptr<DxDevice> device;
	ComPointer<ID3D11SamplerState> samplerState;

	static D3D11_TEXTURE_ADDRESS_MODE ConvertWrap(Wrap wrap);

public:
	DxSampler(ptr<DxDevice> device);

	ID3D11SamplerState* GetSamplerStateInterface() const;

	/// Обновить настройки семплера, если они менялись.
	void Update();
};

END_INANITY_GRAPHICS

#endif
