#include "InterpolateNode.hpp"
#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

InterpolateNode::InterpolateNode(DataType valueType, int semantic, ptr<ValueNode> node)
: valueType(valueType), semantic(semantic), node(node) {}

Node::Type InterpolateNode::GetType() const
{
	return typeInterpolate;
}

DataType InterpolateNode::GetValueType() const
{
	return valueType;
}

int InterpolateNode::GetSemantic() const
{
	return semantic;
}

ptr<ValueNode> InterpolateNode::GetNode() const
{
	return node;
}

END_INANITY_SHADERS
