#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_INSTANCE_HPP___

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

/// Экземпляр генератора шейдеров на GLSL.
class GlslGeneratorInstance
{
private:
	/// Исходный код шейдера.
	ptr<Node> rootNode;
	/// Тип конструируемого шейдера.
	ShaderType shaderType;
	/// Результирующий текст шейдера на GLSL.
	std::ostringstream glsl;

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

public:
	GlslGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType);

	ptr<ShaderSource> Generate();
};

END_INANITY_SHADERS

#endif
