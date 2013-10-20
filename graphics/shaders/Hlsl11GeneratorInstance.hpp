#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL11_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL11_GENERATOR_INSTANCE_HPP___

#include "ShaderType.hpp"
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

/// Экземпляр генератора шейдеров на HLSL.
class Hlsl11GeneratorInstance
{
private:
	/// Root node of shader's source code.
	ptr<Node> rootNode;
	/// Type of the shader.
	ShaderType shaderType;
	/// Text of the shader in HLSL.
	std::ostringstream hlsl;

	/// Attribute variables.
	std::vector<ptr<AttributeNode> > attributes;
	/// Temp variables.
	/** Maps node to index. */
	std::unordered_map<ptr<TempNode>, int> temps;
	int tempsCount;
	bool needInstanceID;
	/// Uniform variables grouped by uniform groups.
	std::vector<std::pair<ptr<UniformGroup>, ptr<UniformNode> > > uniforms;
	std::vector<ptr<SamplerNode> > samplers;
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

public:
	Hlsl11GeneratorInstance(ptr<Node> rootNode, ShaderType shaderType);

	ptr<ShaderSource> Generate();
};

END_INANITY_SHADERS

#endif
