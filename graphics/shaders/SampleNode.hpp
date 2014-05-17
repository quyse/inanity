#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLE_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

class SamplerNode;

class SampleNode : public Node
{
private:
	ptr<SamplerNode> samplerNode;
	ptr<Node> coordsNode;
	ptr<Node> offsetNode;
	// lod, bias and (gradX, gradY) are mutually exclusive.
	ptr<Node> lodNode;
	ptr<Node> biasNode;
	ptr<Node> gradXNode;
	ptr<Node> gradYNode;


public:
	SampleNode(
		ptr<SamplerNode> samplerNode,
		ptr<Node> coordsNode,
		ptr<Node> offsetNode,
		ptr<Node> lodNode,
		ptr<Node> biasNode,
		ptr<Node> gradXNode,
		ptr<Node> gradYNode
	);

	Type GetType() const;

	ptr<SamplerNode> GetSamplerNode() const;
	ptr<Node> GetCoordsNode() const;
	ptr<Node> GetOffsetNode() const;
	ptr<Node> GetLodNode() const;
	ptr<Node> GetBiasNode() const;
	ptr<Node> GetGradXNode() const;
	ptr<Node> GetGradYNode() const;
};

END_INANITY_SHADERS

#endif
