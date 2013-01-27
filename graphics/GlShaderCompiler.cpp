#include "GlShaderCompiler.hpp"
#include "ShaderSource.hpp"
#include "../File.hpp"

ptr<File> GlShaderCompiler::Compile(ptr<ShaderSource> shaderSource)
{
	return shaderSource->GetCode();
}
