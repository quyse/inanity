#ifndef ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___
#define ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

class Node;

/// Untyped shader expression.
/** Stores a node. */
class Expression
{
protected:
	ptr<Node> node;

public:
	Expression(ptr<Node> node = nullptr);

	bool IsValid() const;
	ptr<Node> GetNode() const;
};

Expression operator,(const Expression& a, const Expression& b);

END_INANITY_SHADERS

#endif
