#include "SampleNode.hpp"
#include "SamplerNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

SampleNode::SampleNode(
	ptr<SamplerNode> samplerNode,
	ptr<Node> coordsNode,
	ptr<Node> offsetNode,
	ptr<Node> lodNode,
	ptr<Node> biasNode,
	ptr<Node> gradXNode,
	ptr<Node> gradYNode
) :
	samplerNode(samplerNode),
	coordsNode(coordsNode),
	offsetNode(offsetNode),
	lodNode(lodNode),
	biasNode(biasNode),
	gradXNode(gradXNode),
	gradYNode(gradYNode)
{
	BEGIN_TRY();

	if(lodNode && biasNode || lodNode && gradXNode || biasNode && gradXNode)
		THROW("lod, bias and grad are mutually exclusive");
	if(!!gradXNode != !!gradYNode)
		THROW("gradX and gradY should be set or not set together");

	END_TRY("Can't create sample node");
}

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

ptr<Node> SampleNode::GetOffsetNode() const
{
	return offsetNode;
}

ptr<Node> SampleNode::GetLodNode() const
{
	return lodNode;
}

ptr<Node> SampleNode::GetBiasNode() const
{
	return biasNode;
}

ptr<Node> SampleNode::GetGradXNode() const
{
	return gradXNode;
}

ptr<Node> SampleNode::GetGradYNode() const
{
	return gradYNode;
}

END_INANITY_SHADERS
