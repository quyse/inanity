#include "FragmentNode.hpp"

BEGIN_INANITY_SHADERS

FragmentNode::FragmentNode(int target, ptr<Node> node)
: target(target), node(node) {}

Node::Type FragmentNode::GetType() const
{
	return typeFragment;
}

int FragmentNode::GetTarget() const
{
	return target;
}

ptr<Node> FragmentNode::GetNode() const
{
	return node;
}

END_INANITY_SHADERS
