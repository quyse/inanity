#include "ReadUniformNode.hpp"
#include "UniformNode.hpp"

BEGIN_INANITY_SHADERS

ReadUniformNode::ReadUniformNode(ptr<UniformNode> uniformNode)
: uniformNode(uniformNode) {}

Node::Type ReadUniformNode::GetType() const
{
	return typeReadUniform;
}

DataType ReadUniformNode::GetValueType() const
{
	return uniformNode->GetValueType();
}

ptr<UniformNode> ReadUniformNode::GetUniformNode() const
{
	return uniformNode;
}

END_INANITY_SHADERS
