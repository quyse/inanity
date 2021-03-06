#include "SequenceNode.hpp"

BEGIN_INANITY_SHADERS

SequenceNode::SequenceNode(ptr<Node> a, ptr<Node> b)
: a(a), b(b) {}

Node::Type SequenceNode::GetType() const
{
	return typeSequence;
}

ptr<Node> SequenceNode::GetA() const
{
	return a;
}

ptr<Node> SequenceNode::GetB() const
{
	return b;
}

END_INANITY_SHADERS
