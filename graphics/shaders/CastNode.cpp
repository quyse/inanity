#include "CastNode.hpp"

BEGIN_INANITY_SHADERS

CastNode::CastNode(DataType castDataType, ptr<ValueNode> a)
: castDataType(castDataType), a(a) {}

Node::Type CastNode::GetType() const
{
	return typeCast;
}

DataType CastNode::GetValueType() const
{
	return castDataType;
}

ptr<ValueNode> CastNode::GetA() const
{
	return a;
}

END_INANITY_SHADERS
