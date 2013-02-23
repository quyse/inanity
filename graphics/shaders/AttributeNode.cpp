#include "AttributeNode.hpp"

AttributeNode::AttributeNode(DataType valueType, int semantic)
: valueType(valueType), semantic(semantic) {}

Node::Type AttributeNode::GetType() const
{
	return typeAttribute;
}

DataType AttributeNode::GetValueType() const
{
	return valueType;
}

int AttributeNode::GetSemantic() const
{
	return semantic;
}
