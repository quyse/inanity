#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_GRAPHICS

struct ContextState;
class Texture;
class SamplerState;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

/// Класс узла семплера.
class SamplerNode : public Node
{
private:
	int slot;
	DataType valueType;
	DataType coordType;

public:
	SamplerNode(int slot, DataType valueType, DataType coordType);

	Type GetType() const;

	int GetSlot() const;
	DataType GetValueType() const;
	DataType GetCoordType() const;

	void Apply(ContextState* contextState, ptr<Texture> texture, ptr<SamplerState> samplerState);
};

END_INANITY_SHADERS

#endif
