#ifndef ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_NODE_HPP___

#include "Node.hpp"
#include "Semantic.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла атрибута.
class AttributeNode : public Node
{
protected:
	DataType valueType;
	/// Семантика атрибута - тот же номер, что и в семантике Layout.
	int layoutSemantic;

public:
	AttributeNode(DataType valueType, int layoutSemantic);

	Type GetType() const;

	DataType GetValueType() const;
	int GetLayoutSemantic() const;
	Semantic GetSemantic() const;
};

END_INANITY_SHADERS

#endif
