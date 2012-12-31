#include "TransitionalNode.hpp"

TransitionalNode::TransitionalNode(DataType valueType, TransitionalType transitionalType, Semantic semantic)
: valueType(valueType), transitionalType(transitionalType), semantic(semantic)
{
}

Node::Type TransitionalNode::GetType() const
{
	return transitionalType == transitionalTypeTransformed ? typeTransformed : typeRasterized;
}

DataType TransitionalNode::GetValueType() const
{
	return valueType;
}

TransitionalNode::TransitionalType TransitionalNode::GetTransitionalType() const
{
	return transitionalType;
}

Semantic TransitionalNode::GetSemantic() const
{
	return semantic;
}
