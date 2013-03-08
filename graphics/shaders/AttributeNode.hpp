#ifndef ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла атрибута.
class AttributeNode : public Node
{
protected:
	DataType valueType;
	/// Семантика атрибута.
	int semantic;

public:
	AttributeNode(DataType valueType, int semantic);

	Type GetType() const;

	DataType GetValueType() const;
	int GetSemantic() const;
};

END_INANITY_SHADERS

#endif
