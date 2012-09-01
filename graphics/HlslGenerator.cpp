#include "HlslGenerator.hpp"
#include "shaders/Statement.hpp"
#include "shaders/StatementObjects.hpp"
#include "shaders/Expression.hpp"
#include "shaders/ExpressionObjects.hpp"
#include <sstream>

String HlslGenerator::Generate(const Shaders::Shader& shader)
{
	Statement code = shader.GetCode();

	std::ostringstream hlsl;

	

	Print(hlsl, code);
}
