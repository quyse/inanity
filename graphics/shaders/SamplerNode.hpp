#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_GRAPHICS

class Texture;
class Sampler;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

/// Класс узла семплера.
class SamplerNode : public Node
{
private:
	int slot;
	DataType valueType;
	DataType coordType;
	ptr<Texture> texture;
	ptr<SamplerState> samplerState;

public:
	SamplerNode(int slot, DataType valueType, DataType coordType);

	int GetSlot() const;
	DataType GetValueType() const;
	DataType GetCoordType() const;
};

END_INANITY_SHADERS

#endif
