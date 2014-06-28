#include "GlslGenerator.hpp"
#include "GlslGeneratorInstance.hpp"
#include "../ShaderSource.hpp"
#include "Expression.hpp"
#include "Node.hpp"

BEGIN_INANITY_SHADERS

GlslGenerator::GlslGenerator(GlslVersion glslVersion, bool supportUniformBuffers) :
	glslVersion(glslVersion),
	supportUniformBuffers(supportUniformBuffers)
{}

ptr<ShaderSource> GlslGenerator::Generate(Expression code, ShaderType shaderType)
{
	GlslGeneratorInstance instance(code.GetNode(), shaderType, glslVersion, supportUniformBuffers);

	return instance.Generate();
}

END_INANITY_SHADERS
