#ifndef ___INANITY_GRAPHICS_SHADERS_READ_UNIFORM_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_READ_UNIFORM_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

class UniformNode;

/// Node to read value of uniform non-array variable.
class ReadUniformNode : public ValueNode
{
private:
	ptr<UniformNode> uniformNode;

public:
	ReadUniformNode(ptr<UniformNode> uniformNode);

	Type GetType() const;
	DataType GetValueType() const;

	ptr<UniformNode> GetUniformNode() const;
};

END_INANITY_SHADERS

#endif
