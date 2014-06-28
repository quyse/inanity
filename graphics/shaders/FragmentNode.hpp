#ifndef ___INANITY_GRAPHICS_SHADERS_FRAGMENT_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FRAGMENT_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

class ValueNode;

/// Write fragment color operation.
class FragmentNode : public Node
{
private:
	/// Target number.
	int target;
	/// Node with value.
	ptr<ValueNode> node;

public:
	FragmentNode(int target, ptr<ValueNode> node);

	Type GetType() const;

	int GetTarget() const;
	ptr<ValueNode> GetNode() const;
};

END_INANITY_SHADERS

#endif
