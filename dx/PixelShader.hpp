#ifndef ___INANITY_DX_PIXEL_SHADER_HPP___
#define ___INANITY_DX_PIXEL_SHADER_HPP___

#include "Shader.hpp"
#include "../../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

/// Класс загруженного пиксельного шейдера.
class PixelShader : public Shader
{
private:
	ComPointer<ID3D11PixelShader> pixelShader;

public:
	PixelShader(ptr<ShaderReflection> shaderReflection, ID3D11PixelShader* pixelShader);

	ID3D11PixelShader* GetInternalInterface() const;
};

END_INANITY_DX

#endif
