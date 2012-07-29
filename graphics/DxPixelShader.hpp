#ifndef ___INANITY_GRAPHICS_DX_PIXEL_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX_PIXEL_SHADER_HPP___

#include "PixelShader.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxPixelShader : public PixelShader
{
private:
	ComPointer<ID3D11PixelShader> pixelShader;

public:
	DxPixelShader(ID3D11PixelShader* pixelShader);

	ID3D11PixelShader* GetPixelShaderInterface() const;
};

END_INANITY_GRAPHICS

#endif
