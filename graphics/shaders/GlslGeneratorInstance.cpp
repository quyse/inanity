#include "GlslGeneratorInstance.hpp"
#include "Node.hpp"
#include "AttributeNode.hpp"
#include "FloatConstNode.hpp"
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
#include "../GlslSource.hpp"
#include "../GlShaderBindings.hpp"
#include "../GlSystem.hpp"
#include "../../File.hpp"
#include "../../FileSystem.hpp"
#include "../../Strings.hpp"
#include "../../Exception.hpp"
#include <algorithm>
#include <iomanip>

BEGIN_INANITY_SHADERS

GlslGeneratorInstance::GlslGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType)
: rootNode(rootNode), shaderType(shaderType), tempsCount(0)
{
	try
	{
		switch(shaderType)
		{
		case ShaderTypes::vertex:
			uniformBufferPrefix = "UBv";
			samplerPrefix = "sv";
			break;
		case ShaderTypes::pixel:
			uniformBufferPrefix = "UBp";
			samplerPrefix = "sp";
			break;
		default:
			THROW_PRIMARY_EXCEPTION("Wrong shader type");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create GLSL generator instance", exception);
	}
}

void GlslGeneratorInstance::PrintDataType(DataType dataType)
{
	const char* name;
	switch(dataType)
	{
	case DataTypes::Float:			name = "float";			break;
	case DataTypes::Float2:			name = "vec2";		break;
	case DataTypes::Float3:			name = "vec3";		break;
	case DataTypes::Float4:			name = "vec4";		break;
	case DataTypes::Float3x3:		name = "mat3x3";	break;
	case DataTypes::Float4x4:		name = "mat4x4";	break;
	case DataTypes::UInt:				name = "uint";			break;
	case DataTypes::UInt2:			name = "uvec2";			break;
	case DataTypes::UInt3:			name = "uvec3";			break;
	case DataTypes::UInt4:			name = "uvec4";			break;
	case DataTypes::Int:				name = "int";				break;
	case DataTypes::Int2:				name = "ivec2";			break;
	case DataTypes::Int3:				name = "ivec3";			break;
	case DataTypes::Int4:				name = "ivec4";			break;
	default:
		THROW_PRIMARY_EXCEPTION("Unknown data type");
	}
	glsl << name;
}

void GlslGeneratorInstance::RegisterNode(Node* node)
{
	switch(node->GetType())
	{
	case Node::typeFloatConst:
		break;
	case Node::typeAttribute:
		{
			// атрибуты допустимы только в вершинном шейдере
			if(shaderType != ShaderTypes::vertex)
				THROW_PRIMARY_EXCEPTION("Only vertex shader can have attribute nodes");

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
			THROW_PRIMARY_EXCEPTION("Only pixel shader can have rasterized nodes");
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
		THROW_PRIMARY_EXCEPTION("Unknown node type");
	}
}

void GlslGeneratorInstance::PrintNode(Node* node)
{
	switch(node->GetType())
	{
	case Node::typeFloatConst:
		glsl << std::fixed << std::setprecision(10) << fast_cast<FloatConstNode*>(node)->GetValue() << 'f';
		break;
	case Node::typeAttribute:
		glsl << "a" << fast_cast<AttributeNode*>(node)->GetSemantic();
		break;
	case Node::typeUniform:
		{
			UniformNode* uniformNode = fast_cast<UniformNode*>(node);
			ptr<UniformGroup> uniformGroup = uniformNode->GetGroup();
			glsl << 'u' << uniformGroup->GetSlot() << '_' << uniformNode->GetOffset();
		}
		break;
	case Node::typeSampler:
		// семплер может участвовать, только в операции семплирования
		// он никогда не должен приходить в PrintNode
		THROW_PRIMARY_EXCEPTION("Invalid use of sampler");
	case Node::typeTemp:
		{
			// выяснить номер переменной
			std::unordered_map<ptr<TempNode>, int>::const_iterator i = temps.find(fast_cast<TempNode*>(node));
			if(i == temps.end())
				THROW_PRIMARY_EXCEPTION("Temp node is not registered");

			// напечатать
			glsl << '_' << i->second;
		}
		break;
	case Node::typeTransformed:
		glsl << "v" << fast_cast<TransformedNode*>(node)->GetSemantic();
		break;
	case Node::typeRasterized:
		glsl << "r" << fast_cast<RasterizedNode*>(node)->GetTarget();
		break;
	case Node::typeSequence:
		{
			SequenceNode* sequenceNode = fast_cast<SequenceNode*>(node);
			PrintNode(sequenceNode->GetA());
			glsl << ";\n\t";
			PrintNode(sequenceNode->GetB());
		}
		break;
	case Node::typeSwizzle:
		{
			SwizzleNode* swizzleNode = fast_cast<SwizzleNode*>(node);
			glsl << '(';
			PrintNode(swizzleNode->GetA());
			glsl << ")." << swizzleNode->GetMap();
		}
		break;
	case Node::typeOperation:
		PrintOperationNode(fast_cast<OperationNode*>(node));
		break;
	case Node::typeSample:
		{
			SampleNode* sampleNode = fast_cast<SampleNode*>(node);
			glsl << "(texture(" << samplerPrefix << sampleNode->GetSamplerNode()->GetSlot() << ", ";
			PrintNode(sampleNode->GetCoordsNode());
			glsl << ')';
			// в GLSL всегда возвращается 4-компонентный вектор, так что возможно, нужно получить компоненты
			int valueSize;
			switch(sampleNode->GetSamplerNode()->GetValueType())
			{
			case DataTypes::Float:
			case DataTypes::UInt:
			case DataTypes::Int:
				valueSize = 1;
				break;
			case DataTypes::Float2:
			case DataTypes::UInt2:
			case DataTypes::Int2:
				valueSize = 2;
				break;
			case DataTypes::Float3:
			case DataTypes::UInt3:
			case DataTypes::Int3:
				valueSize = 3;
				break;
			case DataTypes::Float4:
			case DataTypes::UInt4:
			case DataTypes::Int4:
				valueSize = 4;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Invalid sampler value type");
			}
			if(valueSize < 4)
			{
				glsl << '.';
				for(int i = 0; i < valueSize; ++i)
					glsl << "xyzw"[i];
			}
			glsl << ')';
		}
		break;
	case Node::typeCast:
		{
			CastNode* castNode = fast_cast<CastNode*>(node);
			PrintDataType(castNode->GetCastDataType());
			glsl << '(';
			PrintNode(castNode->GetA());
			glsl << ')';
		}
		break;
	default:
		THROW_PRIMARY_EXCEPTION("Unknown node type");
	}
}

void GlslGeneratorInstance::PrintOperationNode(OperationNode* node)
{
	OperationNode::Operation operation = node->GetOperation();
	switch(operation)
	{
	case OperationNode::operationAssign:
		PrintNode(node->GetA());
		glsl << " = (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationIndex:
		PrintNode(node->GetA());
		glsl << '[';
		PrintNode(node->GetB());
		glsl << ']';
		break;
	case OperationNode::operationNegate:
		glsl << "-(";
		PrintNode(node->GetA());
		glsl << ')';
		break;
	case OperationNode::operationAdd:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") + (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationSubtract:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") - (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationMultiply:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") * (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationDivide:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") / (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationLess:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") < (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationLessEqual:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") <= (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationEqual:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") == (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationNotEqual:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") != (";
		PrintNode(node->GetB());
		glsl << ')';
		break;
	case OperationNode::operationSetPosition:
		glsl << "(gl_Position = ";
		PrintNode(node->GetA());
		glsl << ')';
		break;
	case OperationNode::operationGetInstanceID:
		glsl << "uint(gl_InstanceID)";
		break;
	case OperationNode::operationSaturate:
		glsl << "clamp(";
		PrintNode(node->GetA());
		glsl << ", 0, 1)";
		break;
	default:
		{
			// остались только функции, которые делаются простым преобразованием имени
			const char* name;
			switch(operation)
			{
#define OP(o, n) case OperationNode::operation##o: name = #n; break
				OP(Float11to2, vec2);
				OP(Float111to3, vec3);
				OP(Float1111to4, vec4);
				OP(Float31to4, vec4);
				OP(Float211to4, vec4);
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
				OP(Ddx, ddx);
				OP(Ddy, ddy);
				OP(Clip, clip);
#undef OP
			default:
				THROW_PRIMARY_EXCEPTION("Unknown operation type");
			}

			// вывести
			glsl << name << '(';
			int argumentsCount = node->GetArgumentsCount();
			for(int i = 0; i < argumentsCount; ++i)
			{
				if(i)
					glsl << ", ";
				glsl << '(';
				PrintNode(node->GetArgument(i));
				glsl << ')';
			}
			glsl << ')';
		}
	}
}

void GlslGeneratorInstance::PrintUniforms()
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
		glsl << "layout(std140) uniform " << uniformBufferPrefix << slot << "\n{\n";

		// текущее смещение от начала буфера
		int currentOffset = 0;

		// вывести переменные
		for(size_t k = i; k < j; ++k)
		{
			ptr<UniformNode> uniformNode = uniforms[k].second;
			DataType valueType = uniformNode->GetValueType();
			int offset = uniformNode->GetOffset();
			int count = uniformNode->GetCount();
			int valueSize = GetDataTypeSize(valueType);

			// переменная должна лежать на границе float'а, как минимум
			if(offset % sizeof(float))
				THROW_PRIMARY_EXCEPTION("Wrong variable offset: should be on 4-byte boundary");

			// если по умолчанию переменная попадёт в неправильное место, делаем пустые переменные, чтобы занять место
			// автоматический сдвиг
			if(currentOffset % sizeof(float4) + valueSize > sizeof(float4))
				currentOffset = (currentOffset + sizeof(float4) - 1) & ~(sizeof(float4) - 1);
			// оставшийся сдвиг добиваем пустыми переменными
			while(currentOffset < offset)
			{
				int newOffset = (currentOffset + sizeof(float4)) & ~(sizeof(float4) - 1);
				if(newOffset > offset)
					newOffset = offset;
				int size = (newOffset - currentOffset) / sizeof(float);
				static const char* dumpTypes[] = { "float", "vec2", "vec3", "vec4" };
				glsl << '\t' << dumpTypes[size - 1] << " dump" << slot << '_' << currentOffset << '_' << size << ";\n";
				currentOffset = newOffset;
			}

			// печатаем определение переменной

			glsl << '\t';
			PrintDataType(valueType);
			// имя переменной
			glsl << " u" << slot << '_' << offset;

			// размер массива
			if(count > 1)
				glsl << '[' << count << ']';
			// если массив, размер элемента должен быть кратен размеру float4
			if(count > 1 && valueSize % sizeof(float4))
				THROW_PRIMARY_EXCEPTION("Size of element of array should be multiply of float4 size");

			// конец переменной
			glsl << ";\n";

			// смещение для следующей переменной
			currentOffset += valueSize * count;
		}

		// окончание
		glsl << "};\n";

		i = j;
	}
}

ptr<ShaderSource> GlslGeneratorInstance::Generate()
{
	// первый проход: зарегистрировать все переменные
	RegisterNode(rootNode);

	// заголовок файла
	glsl << "#version 150\n";

	// вывести атрибуты в качестве входных переменных, если это вершинный шейдер
	if(shaderType == ShaderTypes::vertex)
	{
		std::sort(attributes.begin(), attributes.end());
		attributes.resize(std::unique(attributes.begin(), attributes.end()) - attributes.begin());
		for(size_t i = 0; i < attributes.size(); ++i)
		{
			ptr<AttributeNode> node = attributes[i];
			glsl << "layout(location = " << node->GetSemantic() << ") in ";
			PrintDataType(node->GetValueType());
			glsl << " a" << node->GetSemantic() << ";\n";
		}
	}
	// вывести промежуточные переменные в любом случае
	std::sort(transformed.begin(), transformed.end());
	transformed.resize(std::unique(transformed.begin(), transformed.end()) - transformed.begin());
	for(size_t i = 0; i < transformed.size(); ++i)
	{
		ptr<TransformedNode> node = transformed[i];
		switch(shaderType)
		{
		case ShaderTypes::vertex:
			glsl << "out ";
			break;
		case ShaderTypes::pixel:
			glsl << "in ";
			break;
		}
		PrintDataType(node->GetValueType());
		glsl << " v" << node->GetSemantic() << ";\n";
	}
	// вывести пиксельные переменные, если это пиксельный шейдер
	if(shaderType == ShaderTypes::pixel)
	{
		std::sort(rasterized.begin(), rasterized.end());
		rasterized.resize(std::unique(rasterized.begin(), rasterized.end()) - rasterized.begin());
		for(size_t i = 0; i < rasterized.size(); ++i)
		{
			ptr<RasterizedNode> node = rasterized[i];
			int target = node->GetTarget();
			glsl << "layout(location = " << target << ") out ";
			PrintDataType(node->GetValueType());
			glsl << " r" << target << ";\n";
		}
	}

	// вывести uniform-буферы
	PrintUniforms();

	// семплеры
	std::sort(samplers.begin(), samplers.end());
	samplers.resize(std::unique(samplers.begin(), samplers.end()) - samplers.begin());
	for(size_t i = 0; i < samplers.size(); ++i)
	{
		ptr<SamplerNode> samplerNode = samplers[i];
		// строка, описывающая основной тип семпла
		const char* valueTypeStr;
		switch(samplerNode->GetValueType())
		{
		case DataTypes::Float:
		case DataTypes::Float2:
		case DataTypes::Float3:
		case DataTypes::Float4:
			valueTypeStr = "";
			break;
		case DataTypes::UInt:
		case DataTypes::UInt2:
		case DataTypes::UInt3:
		case DataTypes::UInt4:
			valueTypeStr = "u";
			break;
		case DataTypes::Int:
		case DataTypes::Int2:
		case DataTypes::Int3:
		case DataTypes::Int4:
			valueTypeStr = "i";
			break;
		default:
			THROW_PRIMARY_EXCEPTION("Invalid sampler value type");
		}
		// строка, описывающая размерность
		const char* dimensionStr;
		switch(samplerNode->GetCoordType())
		{
		case DataTypes::Float:
		case DataTypes::UInt:
		case DataTypes::Int:
			dimensionStr = "1D";
			break;
		case DataTypes::Float2:
			dimensionStr = "2D";
			break;
		case DataTypes::Float3:
			dimensionStr = "3D";
			break;
		default:
			THROW_PRIMARY_EXCEPTION("Invalid sampler coord type");
		}
		// вывести семплер
		int slot = samplerNode->GetSlot();
		glsl << "uniform " << valueTypeStr << "sampler" << dimensionStr << ' ' << samplerPrefix << slot << ";\n";
	}

	//** заголовок функции шейдера

	glsl << "void main()\n{\n";

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
			glsl << '\t';
			PrintDataType(v[i].second);
			glsl << " _" << v[i].first << ";\n";
		}
	}

	glsl << '\t';

	// код шейдера
	PrintNode(rootNode);

	// завершение шейдера
	glsl << ";\n}\n";

	// сформировать список привязок uniform-блоков
	GlShaderBindings::Bindings uniformBlockBindings(uniforms.size());
	for(size_t i = 0; i < uniforms.size(); ++i)
	{
		int slot = uniforms[i].first->GetSlot();
		char name[16];
		sprintf(name, "%s%d", uniformBufferPrefix, slot);
		uniformBlockBindings[i].first = name;
		uniformBlockBindings[i].second = slot;
	}

	// сформировать список привязок семплеров
	GlShaderBindings::Bindings samplerBindings(samplers.size());
	for(size_t i = 0; i < samplers.size(); ++i)
	{
		int slot = samplers[i]->GetSlot();
		char name[16];
		sprintf(name, "%s%d", samplerPrefix, slot);
		samplerBindings[i].first = name;
		samplerBindings[i].second = slot;
	}

	// сформировать список привязок атрибутов
	GlShaderBindings::Bindings attributeBindings(attributes.size());
	for(size_t i = 0; i < attributes.size(); ++i)
	{
		int semantic = attributes[i]->GetSemantic();
		char name[16];
		sprintf(name, "a%d", semantic);
		attributeBindings[i].first = name;
		attributeBindings[i].second = semantic;
	}

	return NEW(GlslSource(
		Strings::String2File(glsl.str()),
		NEW(GlShaderBindings(uniformBlockBindings, samplerBindings, attributeBindings))
	));
}

END_INANITY_SHADERS
