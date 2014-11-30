#ifndef ___INANITY_GRAPHICS_SHADERS_SL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SL_GENERATOR_INSTANCE_HPP___

#include "ShaderType.hpp"
#include "../DataType.hpp"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

BEGIN_INANITY_SHADERS

class Node;
class ValueNode;
class AttributeNode;
class UniformGroup;
class UniformNode;
class SamplerNode;
class TransformedNode;
class InterpolateNode;

/// Base class for HLSL11 and GLSL shader generators.
class SlGeneratorInstance
{
protected:
	/// Root node of shader's source code.
	ptr<Node> rootNode;
	/// Type of the shader.
	ShaderType shaderType;
	/// Text of the shader in language.
	std::ostringstream text;

	/// Registered nodes.
	std::unordered_set<Node*> registeredNodes;
	/// Nodes in order of initialization.
	std::vector<Node*> nodeInits;
	/// Node indices in order of initialization.
	std::unordered_map<Node*, int> nodeInitIndices;
	std::vector<AttributeNode*> attributes;
	/// Univorm variables by groups.
	std::vector<std::pair<UniformGroup*, UniformNode*> > uniforms;
	std::vector<SamplerNode*> samplers;
	std::vector<TransformedNode*> transformedNodes;
	std::vector<InterpolateNode*> interpolateNodes;
	int fragmentTargetsCount;

protected:
	virtual void PrintDataType(DataType dataType) = 0;

	void RegisterNode(Node* node);
	/// Print node representation (actually a temporary variable of node).
	void PrintNode(ValueNode* node);
	void PrintUniform(UniformNode* uniformNode);
	/// Helpers to print node init.
	void PrintNodeInitVar(size_t nodeIndex);
	void PrintNodeInitBegin(size_t nodeIndex);
	void PrintNodeInitEnd();

	SlGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType);
};

END_INANITY_SHADERS

#endif
