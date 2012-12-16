#ifndef ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла атрибута.
class AttributeNode : public Node
{
protected:
	int offset;
	DataType valueType;

public:
	AttributeNode(int offset, DataType valueType);

	Type GetType() const;

	int GetOffset() const;
	DataType GetValueType() const;
};

END_INANITY_SHADERS

#endif
