#include "SpecialNode.hpp"

SpecialNode::SpecialNode(DataType valueType, Semantic semantic)
: valueType(valueType), semantic(semantic) {}

Node::Type SpecialNode::GetType() const
{
	return typeSpecial;
}

DataType SpecialNode::GetValueType() const
{
	return valueType;
}

Semantic SpecialNode::GetSemantic() const
{
	return semantic;
}
