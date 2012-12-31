#ifndef ___INANITY_GRAPHICS_SHADERS_TRANSITIONAL_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_TRANSITIONAL_NODE_HPP___

#include "Node.hpp"
#include "Semantic.hpp"

BEGIN_INANITY_SHADERS

/// Класс промежуточной переменной.
class TransitionalNode : public Node
{
public:
	DataType valueType;

	enum TransitionalType
	{
		transitionalTypeTransformed,
		transitionalTypeRasterized
	};
	TransitionalType transitionalType;

	Semantic semantic;

public:
	TransitionalNode(DataType valueType, TransitionalType transitionalType, Semantic semantic = Semantics::None);

	Type GetType() const;

	DataType GetValueType() const;
	TransitionalType GetTransitionalType() const;
	Semantic GetSemantic() const;
};

END_INANITY_SHADERS

#endif
