#ifndef ___INANITY_GRAPHICS_SHADERS_SPECIAL_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SPECIAL_NODE_HPP___

#include "Node.hpp"
#include "../DataType.hpp"
#include "Semantic.hpp"

BEGIN_INANITY_SHADERS

/// Класс специального узла.
/** Это значение, которое принимается во входных параметрах шейдера. */
class SpecialNode : public Node
{
private:
	DataType valueType;
	Semantic semantic;

public:
	SpecialNode(DataType valueType, Semantic semantic);

	Type GetType() const;

	DataType GetValueType() const;
	Semantic GetSemantic() const;
};

END_INANITY_SHADERS

#endif
