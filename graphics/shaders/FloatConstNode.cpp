#include "FloatConstNode.hpp"

BEGIN_INANITY_SHADERS

FloatConstNode::FloatConstNode(float value)
: value(value) {}

Node::Type FloatConstNode::GetType() const
{
	return typeFloatConst;
}

float FloatConstNode::GetValue() const
{
	return value;
}

END_INANITY_SHADERS
