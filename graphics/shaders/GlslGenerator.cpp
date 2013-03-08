#include "GlslGenerator.hpp"
#include "GlslGeneratorInstance.hpp"
#include "../ShaderSource.hpp"
#include "Expression.hpp"
#include "AttributeNode.hpp"
#include "TempNode.hpp"
#include "SpecialNode.hpp"
#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "SamplerNode.hpp"
#include "TransitionalNode.hpp"
#include "OperationNode.hpp"

ptr<ShaderSource> GlslGenerator::Generate(Expression code, ShaderType shaderType)
{
	GlslGeneratorInstance instance(code.GetNode(), shaderType);

	return instance.Generate();
}
