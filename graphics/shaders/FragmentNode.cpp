#include "FragmentNode.hpp"
#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

FragmentNode::FragmentNode(int target, ptr<ValueNode> node)
: target(target), node(node) {}

Node::Type FragmentNode::GetType() const
{
	return typeFragment;
}

int FragmentNode::GetTarget() const
{
	return target;
}

ptr<ValueNode> FragmentNode::GetNode() const
{
	return node;
}

END_INANITY_SHADERS
