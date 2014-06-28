#ifndef ___INANITY_GRAPHICS_SHADERS_TRANSFORMED_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_TRANSFORMED_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Input variable for pixel shader.
class TransformedNode : public ValueNode
{
private:
	DataType valueType;
	int semantic;

public:
	TransformedNode(DataType valueType, int semantic);

	Type GetType() const;
	DataType GetValueType() const;

	int GetSemantic() const;
};

END_INANITY_SHADERS

#endif
