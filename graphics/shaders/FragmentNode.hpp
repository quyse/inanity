#ifndef ___INANITY_GRAPHICS_SHADERS_FRAGMENT_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FRAGMENT_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Write fragment color operation.
class FragmentNode : public Node
{
private:
	/// Target number.
	int target;
	/// Node with value.
	ptr<Node> node;

public:
	FragmentNode(int target, ptr<Node> node);

	Type GetType() const;

	int GetTarget() const;
	ptr<Node> GetNode() const;
};

END_INANITY_SHADERS

#endif
