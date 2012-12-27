#ifndef ___INANITY_GRAPHICS_DX_SAMPLING_STATE_HPP___
#define ___INANITY_GRAPHICS_DX_SAMPLING_STATE_HPP___

#include "SamplingState.hpp"
#include "../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_GRAPHICS

class DxDevice;

/// Класс семплера DirectX.
class DxSamplingState : public SamplingState
{
private:
	ptr<DxDevice> device;
	ComPointer<ID3D11SamplerState> samplerState;

	static D3D11_TEXTURE_ADDRESS_MODE ConvertWrap(Wrap wrap);

	/// Обновить настройки семплера, если они менялись.
	void Update();

public:
	DxSamplingState(ptr<DxDevice> device);

	ID3D11SamplerState* GetSamplerStateInterface();
};

END_INANITY_GRAPHICS

#endif
