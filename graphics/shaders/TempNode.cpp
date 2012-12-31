#include "TempNode.hpp"

TempNode::TempNode(DataType valueType) : valueType(valueType) {}

Node::Type TempNode::GetType() const
{
	return typeTemp;
}

DataType TempNode::GetValueType() const
{
	return valueType;
}
