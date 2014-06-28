#include "FloatConstNode.hpp"

BEGIN_INANITY_SHADERS

FloatConstNode::FloatConstNode(float value)
: value(value) {}

Node::Type FloatConstNode::GetType() const
{
	return typeFloatConst;
}

DataType FloatConstNode::GetValueType() const
{
	return DataTypes::_float;
}

float FloatConstNode::GetValue() const
{
	return value;
}

END_INANITY_SHADERS
