#ifndef ___INANITY_GRAPHICS_DX_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_DX_TEXTURE_HPP___

#include "Texture.hpp"
#include "../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс текстуры в DirectX 11.
class DxTexture : public Texture
{
private:
	ComPointer<ID3D11ShaderResourceView> shaderResourceView;

public:
	DxTexture(ID3D11ShaderResourceView* shaderResourceView);

	ID3D11ShaderResourceView* GetShaderResourceViewInterface();
};

END_INANITY_GRAPHICS

#endif
