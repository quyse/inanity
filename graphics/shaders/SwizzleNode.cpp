#include "SwizzleNode.hpp"
#include <cstring>

BEGIN_INANITY_SHADERS

SwizzleNode::SwizzleNode(ptr<ValueNode> a, const char* map)
: a(a), map(map) {}

Node::Type SwizzleNode::GetType() const
{
	return typeSwizzle;
}

DataType SwizzleNode::GetValueType() const
{
	return GetVectorDataType(GetBaseDataType(a->GetValueType()), strlen(map));
}

ptr<ValueNode> SwizzleNode::GetA() const
{
	return a;
}

const char* SwizzleNode::GetMap() const
{
	return map;
}

END_INANITY_SHADERS
