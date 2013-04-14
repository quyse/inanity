#include "SampleNode.hpp"
#include "SamplerNode.hpp"

BEGIN_INANITY_SHADERS

SampleNode::SampleNode(ptr<SamplerNode> samplerNode, ptr<Node> coordsNode)
: samplerNode(samplerNode), coordsNode(coordsNode) {}

Node::Type SampleNode::GetType() const
{
	return typeSample;
}

ptr<SamplerNode> SampleNode::GetSamplerNode() const
{
	return samplerNode;
}

ptr<Node> SampleNode::GetCoordsNode() const
{
	return coordsNode;
}

END_INANITY_SHADERS
