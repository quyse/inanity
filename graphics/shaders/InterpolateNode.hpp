#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLATE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLATE_NODE_HPP___

#include "Node.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_SHADERS

class ValueNode;

/// Operation node for outputting results from vertex shader.
class InterpolateNode : public Node
{
private:
	DataType valueType;
	int semantic;
	ptr<ValueNode> node;

public:
	InterpolateNode(DataType valueType, int semantic, ptr<ValueNode> node);

	Type GetType() const;

	DataType GetValueType() const;
	int GetSemantic() const;
	ptr<ValueNode> GetNode() const;
};

END_INANITY_SHADERS

#endif
