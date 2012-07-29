#ifndef ___INANITY_GRAPHICS_DX_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxVertexShader : public VertexShader
{
private:
	ComPointer<ID3D11VertexShader> vertexShader;

public:
	DxVertexShader(ID3D11VertexShader* vertexShader);

	ID3D11VertexShader* GetVertexShaderInterface() const;
};

END_INANITY_GRAPHICS

#endif
