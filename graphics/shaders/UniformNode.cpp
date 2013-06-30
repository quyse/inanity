#include "UniformNode.hpp"
#include "UniformGroup.hpp"

BEGIN_INANITY_SHADERS

UniformNode::UniformNode(ptr<UniformGroup> group, DataType valueType, int offset, int count)
: group(group), valueType(valueType), offset(offset), count(count) {}

Node::Type UniformNode::GetType() const
{
	return typeUniform;
}

ptr<UniformGroup> UniformNode::GetGroup() const
{
	return group;
}

DataType UniformNode::GetValueType() const
{
	return valueType;
}

int UniformNode::GetOffset() const
{
	return offset;
}

int UniformNode::GetCount() const
{
	return count;
}

END_INANITY_SHADERS
