#include "AttributeNode.hpp"

AttributeNode::AttributeNode(DataType valueType, int layoutSemantic)
: valueType(valueType), layoutSemantic(layoutSemantic) {}

Node::Type AttributeNode::GetType() const
{
	return typeAttribute;
}

DataType AttributeNode::GetValueType() const
{
	return valueType;
}

int AttributeNode::GetLayoutSemantic() const
{
	return layoutSemantic;
}

Semantic AttributeNode::GetSemantic() const
{
	return Semantic(Semantics::Custom0 + layoutSemantic);
}
