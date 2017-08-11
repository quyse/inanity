#ifndef ___INANITY_GRAPHICS_SHADERS_DUAL_FRAGMENT_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_DUAL_FRAGMENT_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

class ValueNode;

/// Write dual fragment color operation.
class DualFragmentNode : public Node
{
private:
	ptr<ValueNode> node0, node1;

public:
	DualFragmentNode(ptr<ValueNode> node0, ptr<ValueNode> node1);

	Type GetType() const;

	ptr<ValueNode> GetNode0() const;
	ptr<ValueNode> GetNode1() const;
};

END_INANITY_SHADERS

#endif
