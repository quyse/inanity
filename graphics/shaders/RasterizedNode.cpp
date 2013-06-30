#include "RasterizedNode.hpp"

BEGIN_INANITY_SHADERS

RasterizedNode::RasterizedNode(DataType valueType, int target)
: valueType(valueType), target(target) {}

Node::Type RasterizedNode::GetType() const
{
	return typeRasterized;
}

DataType RasterizedNode::GetValueType() const
{
	return valueType;
}

int RasterizedNode::GetTarget() const
{
	return target;
}

END_INANITY_SHADERS
