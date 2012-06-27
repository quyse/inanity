#ifndef ___INANITY_GRAPHICS_DX_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"

BEGIN_INANITY_GRAPHICS

class DxVertexShader : public VertexShader
{
private:
	ID3D11VertexShader* vertexShader;

public:
	DxVertexShader(ID3D11VertexShader* vertexShader);
};

END_INANITY_GRAPHICS

#endif
