#include "IndexUniformArrayNode.hpp"
#include "UniformNode.hpp"

BEGIN_INANITY_SHADERS

IndexUniformArrayNode::IndexUniformArrayNode(ptr<UniformNode> uniformNode, ptr<ValueNode> indexNode)
: uniformNode(uniformNode), indexNode(indexNode) {}

Node::Type IndexUniformArrayNode::GetType() const
{
	return typeIndexUniformArray;
}

DataType IndexUniformArrayNode::GetValueType() const
{
	return uniformNode->GetValueType();
}

ptr<UniformNode> IndexUniformArrayNode::GetUniformNode() const
{
	return uniformNode;
}

ptr<ValueNode> IndexUniformArrayNode::GetIndexNode() const
{
	return indexNode;
}

END_INANITY_SHADERS
