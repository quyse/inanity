#include "SampleNode.hpp"
#include "SamplerNode.hpp"

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
