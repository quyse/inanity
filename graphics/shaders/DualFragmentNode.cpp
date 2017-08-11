#include "DualFragmentNode.hpp"
#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

DualFragmentNode::DualFragmentNode(ptr<ValueNode> node0, ptr<ValueNode> node1)
: node0(node0), node1(node1) {}

Node::Type DualFragmentNode::GetType() const
{
	return typeDualFragment;
}

ptr<ValueNode> DualFragmentNode::GetNode0() const
{
	return node0;
}

ptr<ValueNode> DualFragmentNode::GetNode1() const
{
	return node1;
}

END_INANITY_SHADERS
