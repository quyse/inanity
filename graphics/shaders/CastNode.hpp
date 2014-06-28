#ifndef ___INANITY_GRAPHICS_SHADERS_CAST_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_CAST_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла преобразования типа.
class CastNode : public ValueNode
{
private:
	DataType castDataType;
	ptr<ValueNode> a;

public:
	CastNode(DataType castDataType, ptr<ValueNode> a);

	Type GetType() const;
	DataType GetValueType() const;

	ptr<ValueNode> GetA() const;
};

END_INANITY_SHADERS

#endif
