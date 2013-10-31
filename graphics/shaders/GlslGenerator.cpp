#include "GlslGenerator.hpp"
#include "GlslGeneratorInstance.hpp"
#include "../ShaderSource.hpp"
#include "Expression.hpp"
#include "AttributeNode.hpp"
#include "TempNode.hpp"
#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "SamplerNode.hpp"
#include "TransformedNode.hpp"
#include "OperationNode.hpp"

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
