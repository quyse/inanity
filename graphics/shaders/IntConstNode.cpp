#include "IntConstNode.hpp"

BEGIN_INANITY_SHADERS

IntConstNode::IntConstNode(int value)
: value(value) {}

Node::Type IntConstNode::GetType() const
{
	return typeIntConst;
}

DataType IntConstNode::GetValueType() const
{
	return DataTypes::_int;
}

int IntConstNode::GetValue() const
{
	return value;
}

END_INANITY_SHADERS
