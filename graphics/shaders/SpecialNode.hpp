#ifndef ___INANITY_GRAPHICS_SHADERS_SPECIAL_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SPECIAL_NODE_HPP___

#include "Node.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_SHADERS

/// Класс специального узла.
/** Это значение, которое принимается во входных параметрах шейдера. */
class SpecialNode : public Node
{
public:
	enum SpecialType
	{
		specialTypeTransformedPosition,
		specialTypeInstance
	};

private:
	DataType valueType;
	SpecialType specialType;

public:
	SpecialNode(DataType valueType, SpecialType specialType);

	Type GetType() const;

	DataType GetValueType() const;
	SpecialType GetSpecialType() const;
};

END_INANITY_SHADERS

#endif
