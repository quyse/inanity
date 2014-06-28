#include "SampleNode.hpp"
#include "SamplerNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

SampleNode::SampleNode(
	ptr<SamplerNode> samplerNode,
	ptr<ValueNode> coordsNode,
	ptr<ValueNode> offsetNode,
	ptr<ValueNode> lodNode,
	ptr<ValueNode> biasNode,
	ptr<ValueNode> gradXNode,
	ptr<ValueNode> gradYNode
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

	if((lodNode && biasNode) || (lodNode && gradXNode) || (biasNode && gradXNode))
		THROW("lod, bias and grad are mutually exclusive");
	if(!!gradXNode != !!gradYNode)
		THROW("gradX and gradY should be set or not set together");

	END_TRY("Can't create sample node");
}

Node::Type SampleNode::GetType() const
{
	return typeSample;
}

DataType SampleNode::GetValueType() const
{
	return samplerNode->GetValueType();
}

ptr<SamplerNode> SampleNode::GetSamplerNode() const
{
	return samplerNode;
}

ptr<ValueNode> SampleNode::GetCoordsNode() const
{
	return coordsNode;
}

ptr<ValueNode> SampleNode::GetOffsetNode() const
{
	return offsetNode;
}

ptr<ValueNode> SampleNode::GetLodNode() const
{
	return lodNode;
}

ptr<ValueNode> SampleNode::GetBiasNode() const
{
	return biasNode;
}

ptr<ValueNode> SampleNode::GetGradXNode() const
{
	return gradXNode;
}

ptr<ValueNode> SampleNode::GetGradYNode() const
{
	return gradYNode;
}

END_INANITY_SHADERS
