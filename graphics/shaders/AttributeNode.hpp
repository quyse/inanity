#ifndef ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_GRAPHICS

class AttributeLayoutElement;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

/// Класс узла атрибута.
class AttributeNode : public Node
{
private:
	ptr<AttributeLayoutElement> element;

public:
	AttributeNode(ptr<AttributeLayoutElement> element);

	Type GetType() const;

	DataType GetValueType() const;
	int GetElementIndex() const;
};

END_INANITY_SHADERS

#endif
