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
	/// Исходный код шейдера.
	ptr<Node> rootNode;
	/// Тип конструируемого шейдера.
	ShaderType shaderType;
	/// Результирующий текст шейдера на HLSL.
	std::ostringstream hlsl;

	/// Атрибутные переменные.
	std::vector<ptr<AttributeNode> > attributes;
	/// Временные переменные.
	/** Мап узел-индекс. */
	std::unordered_map<ptr<TempNode>, int> temps;
	int tempsCount;
	/// Нужен ли шейдеру Instance ID.
	bool needInstanceID;
	/// uniform-переменные по группам.
	std::vector<std::pair<ptr<UniformGroup>, ptr<UniformNode> > > uniforms;
	/// Семплеры.
	std::vector<ptr<SamplerNode> > samplers;
	/// transformed-переменные.
	std::vector<ptr<TransformedNode> > transformed;
	/// rasterized-переменные.
	std::vector<ptr<RasterizedNode> > rasterized;

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