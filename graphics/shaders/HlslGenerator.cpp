#include "HlslGenerator.hpp"
#include "HlslGeneratorInstance.hpp"
#include "../ShaderSource.hpp"
#include "Expression.hpp"
#include "AttributeNode.hpp"
#include "TempNode.hpp"
#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "SamplerNode.hpp"
#include "TransformedNode.hpp"
#include "RasterizedNode.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

ptr<ShaderSource> HlslGenerator::Generate(Expression code, ShaderType shaderType)
{
	HlslGeneratorInstance instance(code.GetNode(), shaderType);

	return instance.Generate();
}

END_INANITY_SHADERS
