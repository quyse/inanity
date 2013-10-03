#include "SamplerNode.hpp"
#include "../Texture.hpp"
#include "../SamplerState.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

SamplerNode::SamplerNode(int slot, DataType valueType, CoordType coordType)
: slot(slot), valueType(valueType), coordType(coordType) {}

Node::Type SamplerNode::GetType() const
{
	return typeSampler;
}

int SamplerNode::GetSlot() const
{
	return slot;
}

DataType SamplerNode::GetValueType() const
{
	return valueType;
}

SamplerNode::CoordType SamplerNode::GetCoordType() const
{
	return coordType;
}

SamplerNode::CoordType SamplerNode::CoordTypeFromDimensions(int dimensions)
{
	switch(dimensions)
	{
	case 1:
		return _1D;
	case 2:
		return _2D;
	case 3:
		return _3D;
	default:
		THROW("Wrong sampler node dimensions");
	}
}

END_INANITY_SHADERS
