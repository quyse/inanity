#ifndef ___INANITY_GRAPHICS_DX_BLEND_STATE_HPP___
#define ___INANITY_GRAPHICS_DX_BLEND_STATE_HPP___

#include "BlendState.hpp"
#include "../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_GRAPHICS

class DxDevice;

/// Класс настроек смешивания DirectX.
class DxBlendState : public BlendState
{
private:
	ptr<DxDevice> device;
	ComPointer<ID3D11BlendState> blendState;

	static D3D11_BLEND ConvertColorSource(ColorSource colorSource);
	static D3D11_BLEND ConvertAlphaSource(AlphaSource alphaSource);
	static D3D11_BLEND_OP ConvertOperation(Operation operation);

	/// Обновить настройки состояния, если они менялись.
	void Update();

public:
	DxBlendState(ptr<DxDevice> device);

	ID3D11BlendState* GetBlendStateInterface();
};

END_INANITY_GRAPHICS

#endif
