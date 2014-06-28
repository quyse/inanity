#include "Expression.hpp"
#include "SequenceNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

Expression::Expression(ptr<Node> node) : node(node) {}

bool Expression::IsValid() const
{
	return node;
}

ptr<Node> Expression::GetNode() const
{
	THROW_ASSERT(node);
	return node;
}

Expression operator,(const Expression& a, const Expression& b)
{
	return Expression(NEW(SequenceNode(a.GetNode(), b.GetNode())));
}

END_INANITY_SHADERS
