#ifndef ___INANITY_GRAPHICS_DX11_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_DX11_TEXTURE_HPP___

#include "Texture.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11SamplerState;

/// Класс текстуры в DirectX 11.
class Dx11Texture : public Texture
{
private:
	ComPointer<ID3D11ShaderResourceView> shaderResourceView;
	ptr<Dx11SamplerState> samplerState;

public:
	Dx11Texture(ComPointer<ID3D11ShaderResourceView> shaderResourceView, ptr<Dx11SamplerState> samplerState);

	ID3D11ShaderResourceView* GetShaderResourceViewInterface();
	ptr<Dx11SamplerState> GetSamplerState() const;
};

END_INANITY_GRAPHICS

#endif
