#include "Hlsl11GeneratorInstance.hpp"
#include "Node.hpp"
#include "AttributeNode.hpp"
#include "FloatConstNode.hpp"
#include "IntConstNode.hpp"
#include "OperationNode.hpp"
#include "SampleNode.hpp"
#include "SamplerNode.hpp"
#include "SequenceNode.hpp"
#include "SwizzleNode.hpp"
#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "TempNode.hpp"
#include "TransformedNode.hpp"
#include "RasterizedNode.hpp"
#include "CastNode.hpp"
#include "../Hlsl11Source.hpp"
#include "../Dx11System.hpp"
#include "../../File.hpp"
#include "../../FileSystem.hpp"
#include "../../Strings.hpp"
#include "../../Exception.hpp"
#include <algorithm>
#include <iomanip>

BEGIN_INANITY_SHADERS

Hlsl11GeneratorInstance::Hlsl11GeneratorInstance(ptr<Node> rootNode, ShaderType shaderType)
: rootNode(rootNode), shaderType(shaderType), tempsCount(0), needInstanceID(false)
{}

void Hlsl11GeneratorInstance::PrintDataType(DataType dataType)
{
	const char* name;
	switch(dataType)
	{
	case DataTypes::_float:		name = "float";			break;
	case DataTypes::_vec2:		name = "float2";		break;
	case DataTypes::_vec3:		name = "float3";		break;
	case DataTypes::_vec4:		name = "float4";		break;
	case DataTypes::_mat3x3:	name = "float3x3";	break;
	case DataTypes::_mat4x4:	name = "float4x4";	break;
	case DataTypes::_uint:		name = "uint";			break;
	case DataTypes::_uvec2:		name = "uint2";			break;
	case DataTypes::_uvec3:		name = "uint3";			break;
	case DataTypes::_uvec4:		name = "uint4";			break;
	case DataTypes::_int:			name = "int";				break;
	case DataTypes::_ivec2:		name = "int2";			break;
	case DataTypes::_ivec3:		name = "int3";			break;
	case DataTypes::_ivec4:		name = "int4";			break;
	default:
		THROW("Unknown data type");
	}
	hlsl << name;
}

void Hlsl11GeneratorInstance::RegisterNode(Node* node)
{
	switch(node->GetType())
	{
	case Node::typeFloatConst:
	case Node::typeIntConst:
		break;
	case Node::typeAttribute:
		{
			// атрибуты допустимы только в вершинном шейдере
			if(shaderType != ShaderTypes::vertex)
				THROW("Only vertex shader can have attribute nodes");

			ptr<AttributeNode> attributeNode = fast_cast<AttributeNode*>(node);
			// просто добавляем, дубликаты удалим потом
			attributes.push_back(attributeNode);
		}
		break;
	case Node::typeUniform:
		{
			UniformNode* uniformNode = fast_cast<UniformNode*>(node);
			// просто добавляем, дубликаты удалим потом
			uniforms.push_back(std::make_pair(uniformNode->GetGroup(), uniformNode));
		}
		break;
	case Node::typeSampler:
		// просто добавляем, дубликаты удалим потом
		samplers.push_back(fast_cast<SamplerNode*>(node));
		break;
	case Node::typeTemp:
		{
			TempNode* tempNode = fast_cast<TempNode*>(node);
			if(temps.find(tempNode) == temps.end())
				temps[tempNode] = tempsCount++;
		}
		break;
	case Node::typeTransformed:
		transformed.push_back(fast_cast<TransformedNode*>(node));
		break;
	case Node::typeRasterized:
		// rasterized-переменые дапустимы только в пиксельном шейдере
		if(shaderType != ShaderTypes::pixel)
			THROW("Only pixel shader can have rasterized nodes");
		rasterized.push_back(fast_cast<RasterizedNode*>(node));
		break;
	case Node::typeSequence:
		{
			SequenceNode* sequenceNode = fast_cast<SequenceNode*>(node);
			RegisterNode(sequenceNode->GetA());
			RegisterNode(sequenceNode->GetB());
		}
		break;
	case Node::typeSwizzle:
		RegisterNode(fast_cast<SwizzleNode*>(node)->GetA());
		break;
	case Node::typeOperation:
		{
			OperationNode* operationNode = fast_cast<OperationNode*>(node);
			int argumentsCount = operationNode->GetArgumentsCount();
			for(int i = 0; i < argumentsCount; ++i)
				RegisterNode(operationNode->GetArgument(i));

			if(operationNode->GetOperation() == OperationNode::operationGetInstanceID)
				needInstanceID = true;
		}
		break;
	case Node::typeSample:
		{
			SampleNode* sampleNode = fast_cast<SampleNode*>(node);
			RegisterNode(sampleNode->GetSamplerNode());
			RegisterNode(sampleNode->GetCoordsNode());
		}
		break;
	case Node::typeCast:
		RegisterNode(fast_cast<CastNode*>(node)->GetA());
		break;
	default:
		THROW("Unknown node type");
	}
}

void Hlsl11GeneratorInstance::PrintNode(Node* node)
{
	switch(node->GetType())
	{
	case Node::typeFloatConst:
		hlsl << std::fixed << std::setprecision(10) << fast_cast<FloatConstNode*>(node)->GetValue() << 'f';
		break;
	case Node::typeIntConst:
		hlsl << fast_cast<IntConstNode*>(node)->GetValue();
		break;
	case Node::typeAttribute:
		hlsl << "a.a" << fast_cast<AttributeNode*>(node)->GetElementIndex();
		break;
	case Node::typeUniform:
		{
			UniformNode* uniformNode = fast_cast<UniformNode*>(node);
			ptr<UniformGroup> uniformGroup = uniformNode->GetGroup();
			hlsl << 'u' << uniformGroup->GetSlot() << '_' << uniformNode->GetOffset();
		}
		break;
	case Node::typeSampler:
		// семплер может участвовать, только в операции семплирования
		// он никогда не должен приходить в PrintNode
		THROW("Invalid use of sampler");
	case Node::typeTemp:
		{
			// выяснить смещение переменной
			std::unordered_map<ptr<TempNode>, int>::const_iterator i = temps.find(fast_cast<TempNode*>(node));
			if(i == temps.end())
				THROW("Temp node is not registered");

			// напечатать
			hlsl << '_' << i->second;
		}
		break;
	case Node::typeTransformed:
		hlsl << "v.v" << fast_cast<TransformedNode*>(node)->GetSemantic();
		break;
	case Node::typeRasterized:
		hlsl << "r.r" << fast_cast<RasterizedNode*>(node)->GetTarget();
		break;
	case Node::typeSequence:
		{
			SequenceNode* sequenceNode = fast_cast<SequenceNode*>(node);
			PrintNode(sequenceNode->GetA());
			hlsl << ";\n\t";
			PrintNode(sequenceNode->GetB());
		}
		break;
	case Node::typeSwizzle:
		{
			SwizzleNode* swizzleNode = fast_cast<SwizzleNode*>(node);
			hlsl << '(';
			PrintNode(swizzleNode->GetA());
			hlsl << ")." << swizzleNode->GetMap();
		}
		break;
	case Node::typeOperation:
		PrintOperationNode(fast_cast<OperationNode*>(node));
		break;
	case Node::typeSample:
		{
			SampleNode* sampleNode = fast_cast<SampleNode*>(node);
			int slot = sampleNode->GetSamplerNode()->GetSlot();
			hlsl << 't' << slot << ".Sample(s" << slot << ", ";
			PrintNode(sampleNode->GetCoordsNode());
			hlsl << ')';
		}
		break;
	case Node::typeCast:
		{
			CastNode* castNode = fast_cast<CastNode*>(node);
			hlsl << '(';
			PrintDataType(castNode->GetCastDataType());
			hlsl << ")(";
			PrintNode(castNode->GetA());
			hlsl << ')';
		}
		break;
	default:
		THROW("Unknown node type");
	}
}

void Hlsl11GeneratorInstance::PrintOperationNode(OperationNode* node)
{
	OperationNode::Operation operation = node->GetOperation();
	switch(operation)
	{
	case OperationNode::operationAssign:
		PrintNode(node->GetA());
		hlsl << " = (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationIndex:
		PrintNode(node->GetA());
		hlsl << '[';
		PrintNode(node->GetB());
		hlsl << ']';
		break;
	case OperationNode::operationNegate:
		hlsl << "-(";
		PrintNode(node->GetA());
		hlsl << ')';
		break;
	case OperationNode::operationAdd:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") + (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationSubtract:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") - (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationMultiply:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") * (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationDivide:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") / (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationLess:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") < (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationLessEqual:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") <= (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationEqual:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") == (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationNotEqual:
		hlsl << '(';
		PrintNode(node->GetA());
		hlsl << ") != (";
		PrintNode(node->GetB());
		hlsl << ')';
		break;
	case OperationNode::operationSetPosition:
		hlsl << "(v.vTP = ";
		PrintNode(node->GetA());
		hlsl << ')';
		break;
	case OperationNode::operationGetInstanceID:
		hlsl << "sI";
		break;
	default:
		{
			// остались только функции, которые делаются простым преобразованием имени
			const char* name;
			switch(operation)
			{
#define OP(o, n) case OperationNode::operation##o: name = #n; break
				OP(Float11to2, float2);
				OP(Float111to3, float3);
				OP(Float1111to4, float4);
				OP(Float31to4, float4);
				OP(Float211to4, float4);
				OP(Dot, dot);
				OP(Cross, cross);
				OP(Mul, mul);
				OP(Length, length);
				OP(Normalize, normalize);
				OP(Pow, pow);
				OP(Min, min);
				OP(Max, max);
				OP(Abs, abs);
				OP(Sin, sin);
				OP(Cos, cos);
				OP(Exp, exp);
				OP(Exp2, exp2);
				OP(Log, log);
				OP(Saturate, saturate);
				OP(Ddx, ddx);
				OP(Ddy, ddy);
				OP(Clip, clip);
#undef OP
			default:
				THROW("Unknown operation type");
			}

			// вывести
			hlsl << name << '(';
			int argumentsCount = node->GetArgumentsCount();
			for(int i = 0; i < argumentsCount; ++i)
			{
				if(i)
					hlsl << ", ";
				hlsl << '(';
				PrintNode(node->GetArgument(i));
				hlsl << ')';
			}
			hlsl << ')';
		}
	}
}

void Hlsl11GeneratorInstance::PrintUniforms()
{
	// uniform-буферы и переменные
	// отсортировать их
	struct Sorter
	{
		bool operator()(const std::pair<ptr<UniformGroup>, ptr<UniformNode> >& a, const std::pair<ptr<UniformGroup>, ptr<UniformNode> >& b) const
		{
			int slotA = a.first->GetSlot();
			int slotB = b.first->GetSlot();

			return slotA < slotB || slotA == slotB && a.second->GetOffset() < b.second->GetOffset();
		}
	};
	std::sort(uniforms.begin(), uniforms.end(), Sorter());
	// удалить дубликаты
	uniforms.resize(std::unique(uniforms.begin(), uniforms.end()) - uniforms.begin());

	// вывести буферы
	for(size_t i = 0; i < uniforms.size(); )
	{
		size_t j;
		for(j = i + 1; j < uniforms.size() && uniforms[j].first == uniforms[i].first; ++j);

		// вывести заголовок
		int slot = uniforms[i].first->GetSlot();
		hlsl << "cbuffer CB" << slot << " : register(b" << slot << ")\n{\n";

		// вывести переменные
		for(size_t k = i; k < j; ++k)
		{
			ptr<UniformNode> uniformNode = uniforms[k].second;
			DataType valueType = uniformNode->GetValueType();
			int offset = uniformNode->GetOffset();
			int count = uniformNode->GetCount();

			// переменная должна лежать на границе float'а, как минимум
			if(offset % sizeof(float))
				THROW("Wrong variable offset: should be on 4-byte boundary");

			// печатаем определение переменной

			hlsl << '\t';
			PrintDataType(valueType);
			// имя переменной
			hlsl << " u" << slot << '_' << offset;

			// размер массива
			if(count > 1)
				hlsl << '[' << count << ']';
			// если массив, размер элемента должен быть кратен размеру vec4
			if(count > 1 && GetDataTypeSize(valueType) % sizeof(vec4))
				THROW("Size of element of array should be multiply of vec4 size");

			// регистр и положение в нём переменной
			hlsl << " : packoffset(c" << (offset / sizeof(vec4));
			// если переменная не начинается ровно на границе регистра, нужно дописать ещё компоненты регистра
			int registerOffset = offset % sizeof(vec4);
			if(registerOffset)
			{
				// получить размер данных
				int variableSize = GetDataTypeSize(valueType);
				// переменная не должна пересекать границу регистра
				if(registerOffset + variableSize > sizeof(vec4))
					THROW("Variable should not intersect a register boundary");
				// выложить столько буков, сколько нужно
				registerOffset /= sizeof(float);
				int endRegisterOffset = registerOffset + variableSize / sizeof(float);
				hlsl << '.';
				for(int j = registerOffset; j < endRegisterOffset; ++j)
					hlsl << "xyzw"[j];
			}
			// конец упаковки
			hlsl << ")";

			// конец переменной
			hlsl << ";\n";
		}

		// окончание
		hlsl << "};\n";

		i = j;
	}
}

ptr<ShaderSource> Hlsl11GeneratorInstance::Generate()
{
	// первый проход: зарегистрировать все переменные
	RegisterNode(rootNode);

	// выборы в зависимости от типа шейдера
	const char* mainFunctionName;
	const char* inputTypeName;
	const char* inputName;
	const char* outputTypeName;
	const char* outputName;
	const char* profile;
	switch(shaderType)
	{
	case ShaderTypes::vertex:
		mainFunctionName = "VS";
		inputTypeName = "A";
		inputName = "a";
		outputTypeName = "V";
		outputName = "v";
		profile = "vs_4_0";
		break;
	case ShaderTypes::pixel:
		mainFunctionName = "PS";
		inputTypeName = "V";
		inputName = "v";
		outputTypeName = "R";
		outputName = "r";
		profile = "ps_4_0";
		break;
	default:
		THROW("Unknown shader type");
	}

	// вывести атрибуты, если вершинный шейдер
	if(shaderType == ShaderTypes::vertex)
	{
		hlsl << "struct A\n{\n";

		struct IndexSorter
		{
			bool operator()(AttributeNode* a, AttributeNode* b) const
			{
				return a->GetElementIndex() < b->GetElementIndex();
			}
		};

		std::sort(attributes.begin(), attributes.end(), IndexSorter());
		attributes.resize(std::unique(attributes.begin(), attributes.end()) - attributes.begin());
		for(size_t i = 0; i < attributes.size(); ++i)
		{
			ptr<AttributeNode> node = attributes[i];
			hlsl << '\t';
			PrintDataType(node->GetValueType());
			int index = node->GetElementIndex();
			hlsl << " a" << index << " : " << Dx11System::GetSemanticString(index) << ";\n";
		}
		hlsl << "};\n";
	}

	// вывести промежуточные переменные в любом случае
	{
		hlsl << "struct V\n{\n";

		// вывести SV_Position
		hlsl << "\tfloat4 vTP : SV_Position;\n";

		struct SemanticSorter
		{
			bool operator()(TransformedNode* a, TransformedNode* b) const
			{
				return a->GetSemantic() < b->GetSemantic();
			}
		};

		std::sort(transformed.begin(), transformed.end(), SemanticSorter());
		transformed.resize(std::unique(transformed.begin(), transformed.end()) - transformed.begin());
		for(size_t i = 0; i < transformed.size(); ++i)
		{
			ptr<TransformedNode> node = transformed[i];
			hlsl << '\t';
			PrintDataType(node->GetValueType());
			int semantic = node->GetSemantic();
			hlsl << " v" << semantic << " : " << Dx11System::GetSemanticString(semantic) << ";\n";
		}
		hlsl << "};\n";
	}

	// вывести пиксельные переменные, если это пиксельный шейдер
	if(shaderType == ShaderTypes::pixel)
	{
		hlsl << "struct R\n{\n";

		struct TargetSorter
		{
			bool operator()(RasterizedNode* a, RasterizedNode* b) const
			{
				return a->GetTarget() < b->GetTarget();
			}
		};

		std::sort(rasterized.begin(), rasterized.end(), TargetSorter());
		rasterized.resize(std::unique(rasterized.begin(), rasterized.end()) - rasterized.begin());
		for(size_t i = 0; i < rasterized.size(); ++i)
		{
			ptr<RasterizedNode> node = rasterized[i];
			hlsl << '\t';
			PrintDataType(node->GetValueType());
			int target = node->GetTarget();
			hlsl << " r" << target << " : SV_Target" << target << ";\n";
		}
		hlsl << "};\n";
	}

	// вывести uniform-буферы
	PrintUniforms();

	// семплеры
	struct SamplerSlotSorter
	{
		bool operator()(SamplerNode* a, SamplerNode* b) const
		{
			return a->GetSlot() < b->GetSlot();
		}
	};
	std::sort(samplers.begin(), samplers.end(), SamplerSlotSorter());
	samplers.resize(std::unique(samplers.begin(), samplers.end()) - samplers.begin());
	for(size_t i = 0; i < samplers.size(); ++i)
	{
		ptr<SamplerNode> samplerNode = samplers[i];
		const char* textureStr;
		switch(samplerNode->GetCoordType())
		{
		case SamplerNode::_1D:
			textureStr = "Texture1D";
			break;
		case SamplerNode::_2D:
			textureStr = "Texture2D";
			break;
		case SamplerNode::_3D:
			textureStr = "Texture3D";
			break;
		case SamplerNode::_Cube:
			textureStr = "TextureCube";
			break;
		default:
			THROW("Invalid sampler coord type");
		}
		// текстура
		hlsl << textureStr << '<';
		PrintDataType(samplerNode->GetValueType());
		int slot = samplerNode->GetSlot();
		hlsl << "> t" << slot << " : register(t" << slot << ");\n";
		// семплер
		hlsl << "SamplerState s" << slot << " : register(s" << slot << ");\n";
	}

	//** заголовок функции шейдера

	hlsl << outputTypeName << ' ' << mainFunctionName << '(' << inputTypeName << ' ' << inputName;

	// если шейдер использует instance ID, добавить аргумент
	if(needInstanceID)
		hlsl << ", uint sI : SV_InstanceID";

	// завершить заголовок
	hlsl << ")\n{\n\t" << outputTypeName << ' ' << outputName << ";\n";

	// временные переменные
	{
		// отсортировать по индексу
		std::vector<std::pair<int, DataType> > v;
		for(std::unordered_map<ptr<TempNode>, int>::const_iterator i = temps.begin(); i != temps.end(); ++i)
			v.push_back(std::make_pair(i->second, i->first->GetValueType()));
		std::sort(v.begin(), v.end());

		// вывести
		for(size_t i = 0; i < v.size(); ++i)
		{
			hlsl << '\t';
			PrintDataType(v[i].second);
			hlsl << " _" << v[i].first << ";\n";
		}
	}

	hlsl << '\t';

	// код шейдера
	PrintNode(rootNode);

	// завершение шейдера
	hlsl << ";\n\treturn " << outputName << ";\n}\n";

	// получить маски ресурсов
	int uniformBuffersMask = 0;
	for(size_t i = 0; i < uniforms.size(); ++i)
		uniformBuffersMask |= 1 << uniforms[i].first->GetSlot();
	int samplersMask = 0;
	for(size_t i = 0; i < samplers.size(); ++i)
		samplersMask |= 1 << samplers[i]->GetSlot();
	Dx11ShaderResources resources(uniformBuffersMask, samplersMask);

	return NEW(Hlsl11Source(Strings::String2File(hlsl.str()), mainFunctionName, profile, resources));
}

END_INANITY_SHADERS
