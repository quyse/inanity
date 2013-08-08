#ifndef ___INANITY_GRAPHICS_DX11_PIXEL_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX11_PIXEL_SHADER_HPP___

#include "PixelShader.hpp"
#include "d3d11.hpp"
#include "Dx11ShaderResources.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11PixelShader : public PixelShader
{
private:
	ComPointer<ID3D11PixelShader> pixelShader;
	Dx11ShaderResources resources;

public:
	Dx11PixelShader(ComPointer<ID3D11PixelShader> pixelShader, const Dx11ShaderResources& resources);

	ID3D11PixelShader* GetPixelShaderInterface() const;
	const Dx11ShaderResources& GetResources() const;
};

END_INANITY_GRAPHICS

#endif
