#include "SamplerNode.hpp"
#include "../ContextState.hpp"
#include "../Texture.hpp"
#include "../SamplerState.hpp"

SamplerNode::SamplerNode(int slot, DataType valueType, DataType coordType)
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

DataType SamplerNode::GetCoordType() const
{
	return coordType;
}

void SamplerNode::Apply(ContextState* contextState, ptr<Texture> texture, ptr<SamplerState> samplerState)
{
	contextState->textures[slot] = texture;
	contextState->samplerStates[slot] = samplerState;
}
