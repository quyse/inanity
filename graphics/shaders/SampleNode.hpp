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

public:
	SampleNode(ptr<SamplerNode> samplerNode, ptr<Node> coordsNode);

	Type GetType() const;

	ptr<SamplerNode> GetSamplerNode() const;
	ptr<Node> GetCoordsNode() const;
};

END_INANITY_SHADERS

#endif
