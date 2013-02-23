#include "TransformedNode.hpp"

TransformedNode::TransformedNode(DataType valueType, int semantic)
: valueType(valueType), semantic(semantic) {}

Node::Type TransformedNode::GetType() const
{
	return typeTransformed;
}

DataType TransformedNode::GetValueType() const
{
	return valueType;
}

int TransformedNode::GetSemantic() const
{
	return semantic;
}
