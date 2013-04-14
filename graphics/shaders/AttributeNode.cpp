#include "AttributeNode.hpp"

BEGIN_INANITY_SHADERS

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

END_INANITY_SHADERS
