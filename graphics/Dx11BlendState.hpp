#ifndef ___INANITY_GRAPHICS_DX11_BLEND_STATE_HPP___
#define ___INANITY_GRAPHICS_DX11_BLEND_STATE_HPP___

#include "BlendState.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс настроек смешивания DirectX.
class Dx11BlendState : public BlendState
{
private:
	ComPointer<ID3D11BlendState> blendState;

	static D3D11_BLEND ConvertColorSource(ColorSource colorSource);
	static D3D11_BLEND ConvertAlphaSource(AlphaSource alphaSource);
	static D3D11_BLEND_OP ConvertOperation(Operation operation);

	/// Обновить настройки состояния, если они менялись.
	void Update(ID3D11Device* deviceInterface);

public:
	ID3D11BlendState* GetBlendStateInterface(ID3D11Device* deviceInterface);
};

END_INANITY_GRAPHICS

#endif
