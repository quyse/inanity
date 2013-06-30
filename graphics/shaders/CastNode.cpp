#include "CastNode.hpp"

BEGIN_INANITY_SHADERS

CastNode::CastNode(DataType castDataType, ptr<Node> a)
: castDataType(castDataType), a(a) {}

Node::Type CastNode::GetType() const
{
	return typeCast;
}

DataType CastNode::GetCastDataType() const
{
	return castDataType;
}

ptr<Node> CastNode::GetA() const
{
	return a;
}

END_INANITY_SHADERS
