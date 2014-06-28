#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_NODE_HPP___

#include "Node.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_SHADERS

/// Abstract node class for representing values.
class ValueNode : public Node
{
public:
	virtual DataType GetValueType() const = 0;
};

END_INANITY_SHADERS

#endif
