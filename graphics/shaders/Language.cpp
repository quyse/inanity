#include "Language.hpp"

BEGIN_INANITY_SHADERS

const Statement operator,(const Statement& a, const Statement& b)
{
	return NEW(CommaStatementObject(a.object, b.object));
}

const TypeInfoOf<float> TypeInfoOf<float>::instance;
const TypeInfoOf<float2> TypeInfoOf<float2>::instance;
const TypeInfoOf<float3> TypeInfoOf<float3>::instance;
const TypeInfoOf<float4> TypeInfoOf<float4>::instance;
const TypeInfoOf<float4x4> TypeInfoOf<float4x4>::instance;
const TypeInfoOf<bool> TypeInfoOf<bool>::instance;

Shader::Shader(Statement code) : code(code) {}

Statement Shader::GetCode() const
{
	return code;
}

END_INANITY_SHADERS
