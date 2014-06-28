#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLE_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

class SamplerNode;

class SampleNode : public ValueNode
{
private:
	ptr<SamplerNode> samplerNode;
	ptr<ValueNode> coordsNode;
	ptr<ValueNode> offsetNode;
	// lod, bias and (gradX, gradY) are mutually exclusive.
	ptr<ValueNode> lodNode;
	ptr<ValueNode> biasNode;
	ptr<ValueNode> gradXNode;
	ptr<ValueNode> gradYNode;


public:
	SampleNode(
		ptr<SamplerNode> samplerNode,
		ptr<ValueNode> coordsNode,
		ptr<ValueNode> offsetNode,
		ptr<ValueNode> lodNode,
		ptr<ValueNode> biasNode,
		ptr<ValueNode> gradXNode,
		ptr<ValueNode> gradYNode
	);

	Type GetType() const;
	DataType GetValueType() const;

	ptr<SamplerNode> GetSamplerNode() const;
	ptr<ValueNode> GetCoordsNode() const;
	ptr<ValueNode> GetOffsetNode() const;
	ptr<ValueNode> GetLodNode() const;
	ptr<ValueNode> GetBiasNode() const;
	ptr<ValueNode> GetGradXNode() const;
	ptr<ValueNode> GetGradYNode() const;
};

END_INANITY_SHADERS

#endif
