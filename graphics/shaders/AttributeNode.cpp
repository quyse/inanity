#include "AttributeNode.hpp"

AttributeNode::AttributeNode(int offset, DataType valueType)
: offset(offset), valueType(valueType) {}

Node::Type AttributeNode::GetType() const
{
	return typeAttribute;
}

int AttributeNode::GetOffset() const
{
	return offset;
}

DataType AttributeNode::GetValueType() const
{
	return valueType;
}
