#include "IntConstNode.hpp"

IntConstNode::IntConstNode(int value)
: value(value) {}

Node::Type IntConstNode::GetType() const
{
	return typeIntConst;
}

int IntConstNode::GetValue() const
{
	return value;
}
