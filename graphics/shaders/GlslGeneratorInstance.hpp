#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___

#include "ShaderType.hpp"
#include "GlslVersion.hpp"
#include "../DataType.hpp"
#include <vector>
#include <unordered_map>
#include <sstream>

BEGIN_INANITY_GRAPHICS

class ShaderSource;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class Node;
class AttributeNode;
class TempNode;
class UniformGroup;
class UniformNode;
class SamplerNode;
class TransformedNode;
class RasterizedNode;
class OperationNode;

/// GLSL shader generator instance.
/** Supports generating shaders with uniform buffers and without. */
class GlslGeneratorInstance
{
private:
	/// Исходный код шейдера.
	ptr<Node> rootNode;
	/// Тип конструируемого шейдера.
	ShaderType shaderType;
	/// Результирующий текст шейдера на GLSL.
	std::ostringstream glsl;

	/// Version of GLSL.
	GlslVersion glslVersion;
	//*** Support flags.
	bool supportUniformBuffers;

	/// Префикс имени uniform-переменной.
	const char* uniformPrefix;
	/// Префикс имени uniform-буфера.
	const char* uniformBufferPrefix;
	/// Префикс имени семплера.
	const char* samplerPrefix;

	/// Атрибутные переменные.
	std::vector<ptr<AttributeNode> > attributes;
	/// Временные переменные.
	/** Мап узел-индекс. */
	std::unordered_map<ptr<TempNode>, int> temps;
	int tempsCount;
	/// uniform-переменные по группам.
	std::vector<std::pair<ptr<UniformGroup>, ptr<UniformNode> > > uniforms;
	/// Семплеры.
	std::vector<ptr<SamplerNode> > samplers;
	/// transformed-переменные.
	std::vector<ptr<TransformedNode> > transformed;
	/// Number of fragment targets.
	int fragmentTargetsCount;

private:
	/// Напечатать имя типа.
	void PrintDataType(DataType dataType);
	/// Зарегистрировать узел.
	void RegisterNode(Node* node);
	/// Напечатать узел.
	void PrintNode(Node* node);
	/// Напечатать узел операции.
	void PrintOperationNode(OperationNode* node);
	/// Вывести uniform-буферы.
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
