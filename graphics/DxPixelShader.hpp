#ifndef ___INANITY_GRAPHICS_DX_PIXEL_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX_PIXEL_SHADER_HPP___

#include "PixelShader.hpp"

BEGIN_INANITY_GRAPHICS

class DxPixelShader : public PixelShader
{
private:
	ID3D11PixelShader* pixelShader;

public:
	DxPixelShader(ID3D11PixelShader* pixelShader);
};

END_INANITY_GRAPHICS

#endif
