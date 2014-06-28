#ifndef ___INANITY_GRAPHICS_SHADERS_INDEX_UNIFORM_ARRAY_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INDEX_UNIFORM_ARRAY_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

class UniformNode;

class IndexUniformArrayNode : public ValueNode
{
private:
	ptr<UniformNode> uniformNode;
	ptr<ValueNode> indexNode;

public:
	IndexUniformArrayNode(ptr<UniformNode> uniformNode, ptr<ValueNode> indexNode);

	Type GetType() const;
	DataType GetValueType() const;

	ptr<UniformNode> GetUniformNode() const;
	ptr<ValueNode> GetIndexNode() const;
};

END_INANITY_SHADERS

#endif
