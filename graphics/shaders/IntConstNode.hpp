#ifndef ___INANITY_GRAPHICS_SHADERS_INT_CONST_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INT_CONST_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла константы.
class IntConstNode : public Node
{
private:
	int value;

public:
	IntConstNode(int value);

	Type GetType() const;

	int GetValue() const;
};

END_INANITY_SHADERS

#endif
