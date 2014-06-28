#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___

#include "ShaderType.hpp"
#include "GlslVersion.hpp"
#include "../DataType.hpp"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

BEGIN_INANITY_GRAPHICS

class ShaderSource;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class Node;
class ValueNode;
class AttributeNode;
class TempNode;
class UniformGroup;
class UniformNode;
class SamplerNode;
class TransformedNode;
class InterpolateNode;
class OperationNode;
class ActionNode;

/// GLSL shader generator instance.
/** Supports generating shaders with uniform buffers and without. */
class GlslGeneratorInstance
{
private:
	ptr<Node> rootNode;
	ShaderType shaderType;
	std::ostringstream glsl;

	/// Version of GLSL.
	GlslVersion glslVersion;
	//*** Support flags.
	bool supportUniformBuffers;

	/// Prefix for uniform variable names.
	const char* uniformPrefix;
	/// Prefix for uniform buffer names.
	const char* uniformBufferPrefix;
	/// Prefix for sampler names.
	const char* samplerPrefix;

	/// Registered nodes.
	std::unordered_set<Node*> registeredNodes;
	/// Nodes in order of initialization.
	std::vector<Node*> nodeInits;
	/// Node indices in order of initialization.
	std::unordered_map<Node*, int> nodeInitIndices;
	std::vector<ptr<AttributeNode> > attributes;
	/// Univorm variables by groups.
	std::vector<std::pair<ptr<UniformGroup>, ptr<UniformNode> > > uniforms;
	std::vector<SamplerNode*> samplers;
	std::vector<TransformedNode*> transformedNodes;
	std::vector<InterpolateNode*> interpolateNodes;
	int fragmentTargetsCount;

private:
	void PrintDataType(DataType dataType);
	void RegisterNode(Node* node);
	void PrintUniform(UniformNode* uniformNode);
	/// Helpers to print node init.
	void PrintNodeInitVar(int nodeIndex);
	void PrintNodeInitBegin(int nodeIndex);
	void PrintNodeInitEnd();
	/// Print node initialization.
	void PrintNodeInit(int nodeIndex);
	void PrintOperationNodeInit(OperationNode* node);
	void PrintActionNodeInit(ActionNode* node);
	/// Print node representation (actually a temporary variable of node).
	void PrintNode(ValueNode* node);
	void PrintUniforms();

	/// Change value type from integer to float.
	static DataType EnforceFloatDataType(DataType dataType);
	/// Print WebGL-needed conversion to integer type.
	bool PrintWebGLConversionToIntegerBegin(DataType dataType);
	void PrintWebGLConversionToIntegerEnd();

public:
	GlslGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType, GlslVersion glslVersion, bool supportUniformBuffers);

	ptr<ShaderSource> Generate();
};

END_INANITY_SHADERS

#endif
