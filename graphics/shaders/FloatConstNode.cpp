#include "FloatConstNode.hpp"

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
