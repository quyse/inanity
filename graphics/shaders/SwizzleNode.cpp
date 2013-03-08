#include "SwizzleNode.hpp"

SwizzleNode::SwizzleNode(ptr<Node> a, const char* map)
: a(a), map(map) {}

Node::Type SwizzleNode::GetType() const
{
	return typeSwizzle;
}

ptr<Node> SwizzleNode::GetA() const
{
	return a;
}

const char* SwizzleNode::GetMap() const
{
	return map;
}
