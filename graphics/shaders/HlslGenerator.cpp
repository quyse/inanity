#include "HlslGenerator.hpp"
#include "HlslGeneratorInstance.hpp"

String HlslGenerator::Generate(const Shaders::Shader& shader)
{
	HlslGeneratorInstance instance(shader);

	return instance.Generate();
}
