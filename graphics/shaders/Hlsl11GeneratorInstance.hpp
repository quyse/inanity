#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL11_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL11_GENERATOR_INSTANCE_HPP___

#include "SlGeneratorInstance.hpp"

BEGIN_INANITY_GRAPHICS

class ShaderSource;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class OperationNode;
class ActionNode;

/// HLSL11 shader generator instance.
class Hlsl11GeneratorInstance : public SlGeneratorInstance
{
private:
	bool needInstanceID;

private:
	//*** SlGeneratorInstance's methods.
	void PrintDataType(DataType dataType);

	void PrintUniform(UniformNode* uniformNode);
	/// Print node initialization.
	void PrintNodeInit(size_t nodeIndex);
	void PrintOperationNodeInit(OperationNode* node);
	void PrintActionNodeInit(ActionNode* node);
	void PrintUniforms();

public:
	Hlsl11GeneratorInstance(ptr<Node> rootNode, ShaderType shaderType);

	ptr<ShaderSource> Generate();
};

END_INANITY_SHADERS

#endif
