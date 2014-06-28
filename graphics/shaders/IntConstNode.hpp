#ifndef ___INANITY_GRAPHICS_SHADERS_INT_CONST_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INT_CONST_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Const with integer value.
class IntConstNode : public ValueNode
{
private:
	int value;

public:
	IntConstNode(int value);

	Type GetType() const;
	DataType GetValueType() const;

	int GetValue() const;
};

END_INANITY_SHADERS

#endif
