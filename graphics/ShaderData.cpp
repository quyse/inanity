#include "ShaderData.h"

BEGIN_INANITY_GRAPHICS

ShaderData::Constant::Constant(unsigned offset, const MultiShaderType& type)
	: offset(offset), type(type)
{
}

const std::vector<ShaderData::Constant>& ShaderData::GetConstants() const
{
	return constants;
}

ptr<MultiShaderConstantNode> ShaderData::AddConstant(size_t offset, const MultiShaderType& type)
{
	constants.push_back(Constant((unsigned)offset, type));
	return NEW(MultiShaderConstantNode(this, constants.size() - 1, type));
}

END_INANITY_GRAPHICS
