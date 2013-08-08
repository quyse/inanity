#include "AttributeNode.hpp"
#include "../AttributeLayoutElement.hpp"

BEGIN_INANITY_SHADERS

AttributeNode::AttributeNode(ptr<AttributeLayoutElement> element)
: element(element) {}

Node::Type AttributeNode::GetType() const
{
	return typeAttribute;
}

DataType AttributeNode::GetValueType() const
{
	return element->GetDataType();
}

int AttributeNode::GetElementIndex() const
{
	return element->GetIndex();
}

END_INANITY_SHADERS
