#ifndef ___INANITY_DX_VERTEX_SHADER_HPP___
#define ___INANITY_DX_VERTEX_SHADER_HPP___

#include "Shader.hpp"
#include "../../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

/// Класс загруженного вершинного шейдера.
class VertexShader : public Shader
{
private:
	ComPointer<ID3D11VertexShader> vertexShader;

public:
	VertexShader(ptr<ShaderReflection> shaderReflection, ID3D11VertexShader* vertexShader);

	ID3D11VertexShader* GetInternalInterface() const;
};

END_INANITY_DX

#endif
