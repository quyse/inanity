#ifndef ___INANITY_GRAPHICS_SHADERS_FLOAT_CONST_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FLOAT_CONST_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Const with float value.
class FloatConstNode : public ValueNode
{
private:
	float value;

public:
	FloatConstNode(float value);

	Type GetType() const;
	DataType GetValueType() const;

	float GetValue() const;
};

END_INANITY_SHADERS

#endif
