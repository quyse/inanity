#ifndef ___INANITY_GRAPHICS_DX11_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_DX11_TEXTURE_HPP___

#include "Texture.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс текстуры в DirectX 11.
class Dx11Texture : public Texture
{
private:
	ComPointer<ID3D11ShaderResourceView> shaderResourceView;

public:
	Dx11Texture(ComPointer<ID3D11ShaderResourceView> shaderResourceView);

	ID3D11ShaderResourceView* GetShaderResourceViewInterface();
};

END_INANITY_GRAPHICS

#endif
