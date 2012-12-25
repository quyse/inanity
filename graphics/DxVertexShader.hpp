#ifndef ___INANITY_GRAPHICS_DX_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class DxVertexShader : public VertexShader
{
private:
	ptr<File> code;
	ComPointer<ID3D11VertexShader> vertexShader;

public:
	DxVertexShader(ptr<File> code, ComPointer<ID3D11VertexShader> vertexShader);

	ptr<Layout> GetAttributesLayout();

	ID3D11VertexShader* GetVertexShaderInterface() const;

	ptr<File> GetCode() const;
};

END_INANITY_GRAPHICS

#endif
