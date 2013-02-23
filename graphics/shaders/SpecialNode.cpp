#include "SpecialNode.hpp"

SpecialNode::SpecialNode(DataType valueType, SpecialType specialType)
: valueType(valueType), specialType(specialType) {}

Node::Type SpecialNode::GetType() const
{
	return typeSpecial;
}

DataType SpecialNode::GetValueType() const
{
	return valueType;
}

SpecialNode::SpecialType SpecialNode::GetSpecialType() const
{
	return specialType;
}
