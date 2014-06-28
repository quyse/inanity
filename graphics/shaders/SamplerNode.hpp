#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_NODE_HPP___

#include "Node.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_GRAPHICS

class Texture;
class SamplerState;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

/// Класс узла семплера.
class SamplerNode : public Node
{
public:
	enum CoordType
	{
		_1D,
		_2D,
		_3D,
		_Cube
	};

private:
	int slot;
	DataType valueType;
	CoordType coordType;

public:
	SamplerNode(int slot, DataType valueType, CoordType coordType);

	Type GetType() const;

	int GetSlot() const;
	DataType GetValueType() const;
	CoordType GetCoordType() const;

	static CoordType CoordTypeFromDimensions(int dimensions);
};

END_INANITY_SHADERS

#endif
