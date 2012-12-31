#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_INSTANCE_HPP___

#include "ShaderType.hpp"
#include "Semantic.hpp"
#include "../DataType.hpp"
#include <vector>
#include <unordered_map>
#include <sstream>

BEGIN_INANITY_SHADERS

class Node;
class AttributeNode;
class TempNode;
class UniformGroup;
class UniformNode;
class SamplerNode;
class TransitionalNode;
class OperationNode;

/// Экземпляр генератора шейдеров на HLSL.
class HlslGeneratorInstance
{
private:
	/// Элемент структурированного буфера (атрибуты, промежуточные и выходные переменные).
	struct Structured
	{
		DataType valueType;
		Semantic semantic;

		Structured(DataType valueType, Semantic semantic);
	};

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
	/// uniform-переменные по группам.
	std::vector<std::pair<ptr<UniformGroup>, ptr<UniformNode> > > uniforms;
	/// Семплеры.
	std::vector<ptr<SamplerNode> > samplers;
	/// transformed-переменные.
	std::vector<ptr<TransitionalNode> > transformed;
	/// rasterized-переменные.
	std::vector<ptr<TransitionalNode> > rasterized;

private:
	/// Напечатать имя типа.
	void PrintDataType(DataType dataType);
	/// Зарегистрировать узел.
	void RegisterNode(Node* node);
	/// Напечатать узел.
	void PrintNode(Node* node);
	/// Напечатать узел операции.
	void PrintOperationNode(OperationNode* node);
	/// Вывести структуру.
	void PrintStructure(const std::vector<Structured>& elements, const char* variableNamePrefix);
	/// Вывести uniform-буферы.
	void PrintUniforms();

	template <typename NodeType>
	static void ProcessTransitionalNodes(std::vector<ptr<NodeType> >& nodes, std::vector<Structured>& structure);

public:
	HlslGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType);

	std::string Generate();
};

END_INANITY_SHADERS

#endif
