#include "Hlsl11Generator.hpp"
#include "Hlsl11GeneratorInstance.hpp"
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

ptr<ShaderSource> Hlsl11Generator::Generate(Expression code, ShaderType shaderType)
{
	Hlsl11GeneratorInstance instance(code.GetNode(), shaderType);

	return instance.Generate();
}

END_INANITY_SHADERS
