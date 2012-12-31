#include "UniformNode.hpp"
#include "UniformGroup.hpp"

UniformNode::UniformNode(ptr<UniformGroup> group, DataType valueType, int offset)
: group(group), valueType(valueType), offset(offset) {}

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
