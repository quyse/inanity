#ifndef ___INANITY_GRAPHICS_DX11_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX11_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"
#include "d3d11.hpp"
#include "Dx11ShaderResources.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class Dx11VertexShader : public VertexShader
{
private:
	ptr<File> code;
	ComPointer<ID3D11VertexShader> vertexShader;
	Dx11ShaderResources resources;

public:
	Dx11VertexShader(ptr<File> code, ComPointer<ID3D11VertexShader> vertexShader, const Dx11ShaderResources& resources);

	ID3D11VertexShader* GetVertexShaderInterface() const;
	ptr<File> GetCode() const;
	const Dx11ShaderResources& GetResources() const;
};

END_INANITY_GRAPHICS

#endif
