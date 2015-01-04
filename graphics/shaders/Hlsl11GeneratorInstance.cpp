#include "Hlsl11GeneratorInstance.hpp"
#include "AttributeNode.hpp"
#include "FloatConstNode.hpp"
#include "IntConstNode.hpp"
#include "OperationNode.hpp"
#include "ActionNode.hpp"
#include "SampleNode.hpp"
#include "SamplerNode.hpp"
#include "SequenceNode.hpp"
#include "SwizzleNode.hpp"
#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "ReadUniformNode.hpp"
#include "IndexUniformArrayNode.hpp"
#include "TransformedNode.hpp"
#include "InterpolateNode.hpp"
#include "FragmentNode.hpp"
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

Hlsl11GeneratorInstance::Hlsl11GeneratorInstance(ptr<Node> rootNode, ShaderType shaderType) :
	SlGeneratorInstance(rootNode, shaderType),
	needInstanceID(false)
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
	case DataTypes::_bool:		name = "bool";			break;
	case DataTypes::_bvec2:		name = "bool2";			break;
	case DataTypes::_bvec3:		name = "bool3";			break;
	case DataTypes::_bvec4:		name = "bool4";			break;
	default:
		THROW("Unknown data type");
	}
	text << name;
}

void Hlsl11GeneratorInstance::PrintUniform(UniformNode* uniformNode)
{
	text << 'u' << uniformNode->GetGroup()->GetSlot() << '_' << uniformNode->GetOffset();
}

void Hlsl11GeneratorInstance::PrintNodeInit(size_t nodeIndex)
{
	Node* node = nodeInits[nodeIndex];

	switch(node->GetType())
	{
	case Node::typeFloatConst:
		PrintNodeInitBegin(nodeIndex);
		text << std::fixed << std::setprecision(10) << fast_cast<FloatConstNode*>(node)->GetValue() << 'f';
		PrintNodeInitEnd();
		break;
	case Node::typeIntConst:
		PrintNodeInitBegin(nodeIndex);
		text << fast_cast<IntConstNode*>(node)->GetValue();
		PrintNodeInitEnd();
		break;
	case Node::typeAttribute:
		PrintNodeInitBegin(nodeIndex);
		text << "a.a" << fast_cast<AttributeNode*>(node)->GetElementIndex();
		PrintNodeInitEnd();
		break;
	case Node::typeUniform:
		// uniform doesn't have initialization
		break;
	case Node::typeSampler:
		// sampler doesn't have initialization
		break;
	case Node::typeReadUniform:
		{
			PrintNodeInitBegin(nodeIndex);
			ReadUniformNode* readUniformNode = fast_cast<ReadUniformNode*>(node);
			PrintUniform(readUniformNode->GetUniformNode());
			PrintNodeInitEnd();
		}
		break;
	case Node::typeIndexUniformArray:
		{
			PrintNodeInitBegin(nodeIndex);
			IndexUniformArrayNode* indexUniformArrayNode = fast_cast<IndexUniformArrayNode*>(node);
			PrintUniform(indexUniformArrayNode->GetUniformNode());
			text << '[';
			PrintNode(indexUniformArrayNode->GetIndexNode());
			text << ']';
			PrintNodeInitEnd();
		}
		break;
	case Node::typeTransformed:
		PrintNodeInitBegin(nodeIndex);
		text << "v.v" << fast_cast<TransformedNode*>(node)->GetSemantic();
		PrintNodeInitEnd();
		break;
	case Node::typeInterpolate:
		{
			InterpolateNode* interpolateNode = fast_cast<InterpolateNode*>(node);
			text << "\tv.v" << interpolateNode->GetSemantic() << " = ";
			PrintNode(interpolateNode->GetNode());
			PrintNodeInitEnd();
		}
		break;
	case Node::typeSequence:
		// sequence node doesn't have initialization
		break;
	case Node::typeSwizzle:
		{
			PrintNodeInitBegin(nodeIndex);
			SwizzleNode* swizzleNode = fast_cast<SwizzleNode*>(node);
			PrintNode(swizzleNode->GetA());
			text << '.' << swizzleNode->GetMap();
			PrintNodeInitEnd();
		}
		break;
	case Node::typeOperation:
		PrintNodeInitBegin(nodeIndex);
		PrintOperationNodeInit(fast_cast<OperationNode*>(node));
		PrintNodeInitEnd();
		break;
	case Node::typeAction:
		PrintActionNodeInit(fast_cast<ActionNode*>(node));
		break;
	case Node::typeSample:
		{
			PrintNodeInitBegin(nodeIndex);

			SampleNode* sampleNode = fast_cast<SampleNode*>(node);

			int slot = sampleNode->GetSamplerNode()->GetSlot();
			text << 't' << slot;

			ValueNode* coordsNode = sampleNode->GetCoordsNode();
			ValueNode* offsetNode = sampleNode->GetOffsetNode();
			ValueNode* lodNode = sampleNode->GetLodNode();
			ValueNode* biasNode = sampleNode->GetBiasNode();
			ValueNode* gradXNode = sampleNode->GetGradXNode();
			ValueNode* gradYNode = sampleNode->GetGradYNode();

			if(lodNode)
			{
				text << ".SampleLevel(s" << slot << ", ";
				PrintNode(coordsNode);
				text << ", ";
				PrintNode(lodNode);
			}
			else if(biasNode)
			{
				text << ".SampleBias(s" << slot << ", ";
				PrintNode(coordsNode);
				text << ", ";
				PrintNode(biasNode);
			}
			else if(gradXNode && gradYNode)
			{
				text << ".SampleGrad(s" << slot << ", ";
				PrintNode(coordsNode);
				text << ", ";
				PrintNode(gradXNode);
				text << ", ";
				PrintNode(gradYNode);
			}
			else
			{
				text << ".Sample(s" << slot << ", ";
				PrintNode(coordsNode);
			}

			// add offset if needed
			if(offsetNode)
			{
				text << ", ";
				PrintNode(offsetNode);
			}

			text << ')';

			PrintNodeInitEnd();
		}
		break;
	case Node::typeFragment:
		{
			FragmentNode* fragmentNode = fast_cast<FragmentNode*>(node);
			text << "\tr.r" << fragmentNode->GetTarget() << " = ";
			PrintNode(fragmentNode->GetNode());
			PrintNodeInitEnd();
		}
		break;
	case Node::typeCast:
		{
			PrintNodeInitBegin(nodeIndex);
			CastNode* castNode = fast_cast<CastNode*>(node);
			text << '(';
			PrintDataType(castNode->GetValueType());
			text << ")";
			PrintNode(castNode->GetA());
			PrintNodeInitEnd();
		}
		break;
	default:
		THROW("Unknown node type");
	}
}

void Hlsl11GeneratorInstance::PrintOperationNodeInit(OperationNode* node)
{
	OperationNode::Operation operation = node->GetOperation();
	switch(operation)
	{
	case OperationNode::operationNegate:
		text << '-';
		PrintNode(node->GetA());
		break;
	case OperationNode::operationAdd:
		PrintNode(node->GetA());
		text << " + ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationSubtract:
		PrintNode(node->GetA());
		text << " - ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationMultiply:
		PrintNode(node->GetA());
		text << " * ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationDivide:
		PrintNode(node->GetA());
		text << " / ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationLess:
		PrintNode(node->GetA());
		text << " < ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationLessEqual:
		PrintNode(node->GetA());
		text << " <= ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationEqual:
		PrintNode(node->GetA());
		text << " == ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationNotEqual:
		PrintNode(node->GetA());
		text << " != ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationGetInstanceID:
		text << "sI";
		break;
	case OperationNode::operationScreenToTexture:
		PrintNode(node->GetA());
		text << " * float2(0.5f, -0.5f) + float2(0.5f, 0.5f)";
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
				OP(Lerp, lerp);
				OP(Pow, pow);
				OP(Min, min);
				OP(Max, max);
				OP(Abs, abs);
				OP(Sqrt, sqrt);
				OP(Sin, sin);
				OP(Cos, cos);
				OP(Atan2, atan2);
				OP(Exp, exp);
				OP(Exp2, exp2);
				OP(Log, log);
				OP(Saturate, saturate);
				OP(Ddx, ddx);
				OP(Ddy, ddy);
				OP(Floor, floor);
				OP(Ceil, ceil);
				OP(Mod, fmod);
#undef OP
			default:
				THROW("Invalid operation type");
			}

			// вывести
			text << name << '(';
			int argumentsCount = node->GetArgumentsCount();
			for(int i = 0; i < argumentsCount; ++i)
			{
				if(i)
					text << ", ";
				PrintNode(node->GetArgument(i));
			}
			text << ')';
		}
	}
}

void Hlsl11GeneratorInstance::PrintActionNodeInit(ActionNode* node)
{
	switch(node->GetAction())
	{
	case ActionNode::actionSetPosition:
		text << "\tv.vTP = ";
		PrintNode(node->GetA());
		PrintNodeInitEnd();
		break;
	case ActionNode::actionClip:
		text << "\tclip(";
		PrintNode(node->GetA());
		text << ')';
		PrintNodeInitEnd();
		break;
	default:
		THROW("Invalid action type");
	}
}

void Hlsl11GeneratorInstance::PrintUniforms()
{
	// uniform-буферы и переменные
	// отсортировать их
	struct Sorter
	{
		bool operator()(const std::pair<UniformGroup*, UniformNode*>& a, const std::pair<UniformGroup*, UniformNode*>& b) const
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
		text << "cbuffer CB" << slot << " : register(b" << slot << ")\n{\n";

		// вывести переменные
		for(size_t k = i; k < j; ++k)
		{
			UniformNode* uniformNode = uniforms[k].second;
			DataType valueType = uniformNode->GetValueType();
			int offset = uniformNode->GetOffset();
			int count = uniformNode->GetCount();

			// переменная должна лежать на границе float'а, как минимум
			if(offset % sizeof(float))
				THROW("Wrong variable offset: should be on 4-byte boundary");

			// печатаем определение переменной

			text << '\t';
			PrintDataType(valueType);
			// имя переменной
			text << " u" << slot << '_' << offset;

			// размер массива
			if(count > 1)
				text << '[' << count << ']';
			// если массив, размер элемента должен быть кратен размеру vec4
			if(count > 1 && GetDataTypeSize(valueType) % sizeof(vec4))
				THROW("Size of element of array should be multiply of vec4 size");

			// регистр и положение в нём переменной
			text << " : packoffset(c" << (offset / sizeof(vec4));
			// если переменная не начинается ровно на границе регистра, нужно дописать ещё первую компоненту регистра
			int registerOffset = offset % sizeof(vec4);
			if(registerOffset)
			{
				// получить размер данных
				int variableSize = GetDataTypeSize(valueType);
				// переменная не должна пересекать границу регистра
				if(registerOffset + variableSize > sizeof(vec4))
					THROW("Variable should not intersect a register boundary");
				// выложить нужную букву
				registerOffset /= sizeof(float);
				text << '.' << "xyzw"[registerOffset];
			}
			// конец упаковки
			text << ")";

			// конец переменной
			text << ";\n";
		}

		// окончание
		text << "};\n";

		i = j;
	}
}

ptr<ShaderSource> Hlsl11GeneratorInstance::Generate()
{
	// first stage: register all nodes
	RegisterNode(rootNode);

	// find out if we need instance id
	for(size_t i = 0; i < nodeInits.size(); ++i)
	{
		Node* node = nodeInits[i];
		if(node->GetType() == Node::typeOperation)
		{
			OperationNode* operationNode = fast_cast<OperationNode*>(node);
			if(operationNode->GetOperation() == OperationNode::operationGetInstanceID)
				needInstanceID = true;
		}
	}

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
		text << "struct A\n{\n";

		struct IndexSorter
		{
			bool operator()(AttributeNode* a, AttributeNode* b) const
			{
				return a->GetElementIndex() < b->GetElementIndex();
			}
		};

		std::sort(attributes.begin(), attributes.end(), IndexSorter());
		for(size_t i = 0; i < attributes.size(); ++i)
		{
			AttributeNode* node = attributes[i];
			text << '\t';
			PrintDataType(node->GetValueType());
			int index = node->GetElementIndex();
			text << " a" << index << " : " << Dx11System::GetSemanticString(index) << ";\n";
		}
		text << "};\n";
	}

	// print transformed nodes
	if(shaderType == ShaderTypes::pixel)
	{
		text << "struct V\n{\n";

		struct SemanticSorter
		{
			bool operator()(TransformedNode* a, TransformedNode* b) const
			{
				return a->GetSemantic() < b->GetSemantic();
			}
		};

		std::sort(transformedNodes.begin(), transformedNodes.end(), SemanticSorter());

		for(size_t i = 0; i < transformedNodes.size(); ++i)
		{
			TransformedNode* node = transformedNodes[i];
			text << '\t';
			PrintDataType(node->GetValueType());
			int semantic = node->GetSemantic();
			text << " v" << semantic << " : " << Dx11System::GetSemanticString(semantic) << ";\n";
		}

		text << "};\n";
	}

	// print interpolate nodes
	if(shaderType == ShaderTypes::vertex)
	{
		text << "struct V\n{\n";

		struct SemanticSorter
		{
			bool operator()(InterpolateNode* a, InterpolateNode* b) const
			{
				return a->GetSemantic() < b->GetSemantic();
			}
		};

		std::sort(interpolateNodes.begin(), interpolateNodes.end(), SemanticSorter());

		for(size_t i = 0; i < interpolateNodes.size(); ++i)
		{
			InterpolateNode* node = interpolateNodes[i];
			text << '\t';
			PrintDataType(node->GetValueType());
			int semantic = node->GetSemantic();
			text << " v" << semantic << " : " << Dx11System::GetSemanticString(semantic) << ";\n";
		}

		// вывести SV_Position
		text << "\tfloat4 vTP : SV_Position;\n";

		text << "};\n";
	}

	// вывести пиксельные переменные, если это пиксельный шейдер
	if(shaderType == ShaderTypes::pixel)
	{
		text << "struct R\n{\n";

		for(int i = 0; i < fragmentTargetsCount; ++i)
		{
			text << '\t';
			PrintDataType(DataTypes::_vec4);
			text << " r" << i << " : SV_Target" << i << ";\n";
		}

		text << "};\n";
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
	for(size_t i = 0; i < samplers.size(); ++i)
	{
		SamplerNode* samplerNode = samplers[i];
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
		text << textureStr << '<';
		PrintDataType(samplerNode->GetValueType());
		int slot = samplerNode->GetSlot();
		text << "> t" << slot << " : register(t" << slot << ");\n";
		// семплер
		text << "SamplerState s" << slot << " : register(s" << slot << ");\n";
	}

	//** заголовок функции шейдера

	text << outputTypeName << ' ' << mainFunctionName << '(' << inputTypeName << ' ' << inputName;

	// если шейдер использует instance ID, добавить аргумент
	if(needInstanceID)
		text << ", uint sI : SV_InstanceID";

	// завершить заголовок
	text << ")\n{\n\t" << outputTypeName << ' ' << outputName << ";\n";

	// shader code
	for(size_t i = 0; i < nodeInits.size(); ++i)
		PrintNodeInit(i);

	// завершение шейдера
	text << "\treturn " << outputName << ";\n}\n";

	// получить маски ресурсов
	int uniformBuffersMask = 0;
	for(size_t i = 0; i < uniforms.size(); ++i)
		uniformBuffersMask |= 1 << uniforms[i].first->GetSlot();
	int samplersMask = 0;
	for(size_t i = 0; i < samplers.size(); ++i)
		samplersMask |= 1 << samplers[i]->GetSlot();
	Dx11ShaderResources resources(uniformBuffersMask, samplersMask);

	return NEW(Hlsl11Source(Strings::String2File(text.str()), mainFunctionName, profile, resources));
}

END_INANITY_SHADERS
