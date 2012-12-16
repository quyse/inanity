#include "SamplerNode.hpp"

SamplerNode::SamplerNode(int slot, DataType valueType, DataType coordType)
: slot(slot), valueType(valueType), coordType(coordType) {}

int SamplerNode::GetSlot() const
{
	return slot;
}

DataType SamplerNode::GetValueType() const
{
	return valueType;
}

DataType SamplerNode::GetCoordType() const
{
	return coordType;
}
