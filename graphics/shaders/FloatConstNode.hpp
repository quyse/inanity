#ifndef ___INANITY_GRAPHICS_SHADERS_FLOAT_CONST_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FLOAT_CONST_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла константы.
class FloatConstNode : public Node
{
private:
	float value;

public:
	FloatConstNode(float value);

	Type GetType() const;

	float GetValue() const;
};

END_INANITY_SHADERS

#endif
