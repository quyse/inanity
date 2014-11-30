#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___

#include "SlGeneratorInstance.hpp"
#include "GlslVersion.hpp"

BEGIN_INANITY_GRAPHICS

class ShaderSource;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class OperationNode;
class ActionNode;

/// GLSL shader generator instance.
/** Supports generating shaders with uniform buffers and without. */
class GlslGeneratorInstance : SlGeneratorInstance
{
private:
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

private:
	//*** SlGeneratorInstance's methods.
	void PrintDataType(DataType dataType);

	void PrintUniform(UniformNode* uniformNode);
	/// Print node initialization.
	void PrintNodeInit(size_t nodeIndex);
	void PrintOperationNodeInit(OperationNode* node);
	void PrintActionNodeInit(ActionNode* node);
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
