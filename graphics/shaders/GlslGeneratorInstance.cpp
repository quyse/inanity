#include "GlslGeneratorInstance.hpp"
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
#include "FragmentNode.hpp"
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

GlslGeneratorInstance::GlslGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType, GlslVersion glslVersion, bool supportUniformBuffers) :
	rootNode(rootNode),
	shaderType(shaderType),
	glslVersion(glslVersion),
	supportUniformBuffers(supportUniformBuffers),
	tempsCount(0),
	fragmentTargetsCount(0)
{
	try
	{
		switch(shaderType)
		{
		case ShaderTypes::vertex:
			uniformPrefix = "uv";
			uniformBufferPrefix = "UBv";
			samplerPrefix = "sv";
			break;
		case ShaderTypes::pixel:
			uniformPrefix = "up";
			uniformBufferPrefix = "UBp";
			samplerPrefix = "sp";
			break;
		default:
			THROW("Wrong shader type");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create GLSL generator instance", exception);
	}
}

void GlslGeneratorInstance::PrintDataType(DataType dataType)
{
	const char* name;
	switch(dataType)
	{
	case DataTypes::_float:			name = "float";			break;
	case DataTypes::_vec2:			name = "vec2";			break;
	case DataTypes::_vec3:			name = "vec3";			break;
	case DataTypes::_vec4:			name = "vec4";			break;
	case DataTypes::_mat3x3:		name = "mat3";		break;
	case DataTypes::_mat4x4:		name = "mat4";		break;
	case DataTypes::_uint:			name = glslVersion == GlslVersions::webgl ? "int" : "uint"; break;
	case DataTypes::_uvec2:			name = glslVersion == GlslVersions::webgl ? "ivec2" : "uvec2"; break;
	case DataTypes::_uvec3:			name = glslVersion == GlslVersions::webgl ? "ivec3" : "uvec3"; break;
	case DataTypes::_uvec4:			name = glslVersion == GlslVersions::webgl ? "ivec4" : "uvec4"; break;
	case DataTypes::_int:				name = "int";				break;
	case DataTypes::_ivec2:			name = "ivec2";			break;
	case DataTypes::_ivec3:			name = "ivec3";			break;
	case DataTypes::_ivec4:			name = "ivec4";			break;
	default:
		THROW("Unknown data type");
	}
	glsl << name;
}

void GlslGeneratorInstance::RegisterNode(Node* node)
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
	case Node::typeFragment:
		{
			// fragment output is allowed only in pixel shader
			if(shaderType != ShaderTypes::pixel)
				THROW("Only pixel shader can do fragment output");
			FragmentNode* fragmentNode = fast_cast<FragmentNode*>(node);
			// register maximum number of fragment outputs
			fragmentTargetsCount = std::max(fragmentTargetsCount, fragmentNode->GetTarget() + 1);
			RegisterNode(fragmentNode->GetNode());
		}
		break;
	case Node::typeCast:
		RegisterNode(fast_cast<CastNode*>(node)->GetA());
		break;
	default:
		THROW("Unknown node type");
	}
}

void GlslGeneratorInstance::PrintNode(Node* node)
{
	switch(node->GetType())
	{
	case Node::typeFloatConst:
		glsl << std::fixed << std::setprecision(10) << fast_cast<FloatConstNode*>(node)->GetValue();// << 'f';
		break;
	case Node::typeIntConst:
		glsl << fast_cast<IntConstNode*>(node)->GetValue();
		break;
	case Node::typeAttribute:
		{
			// WebGL hack for integer attributes
			// (only for case of using another hack - changing integer attributes to float)
			bool intConversion = false;
			if(glslVersion == GlslVersions::webgl)
				intConversion = PrintWebGLConversionToIntegerBegin(fast_cast<AttributeNode*>(node)->GetValueType());

			glsl << "a" << fast_cast<AttributeNode*>(node)->GetElementIndex();

			if(intConversion)
				PrintWebGLConversionToIntegerEnd();
		}
		break;
	case Node::typeUniform:
		{
			UniformNode* uniformNode = fast_cast<UniformNode*>(node);
			ptr<UniformGroup> uniformGroup = uniformNode->GetGroup();
			glsl << uniformPrefix << uniformGroup->GetSlot() << '_' << uniformNode->GetOffset();
		}
		break;
	case Node::typeSampler:
		// семплер может участвовать, только в операции семплирования
		// он никогда не должен приходить в PrintNode
		THROW("Invalid use of sampler");
	case Node::typeTemp:
		{
			// выяснить номер переменной
			std::unordered_map<ptr<TempNode>, int>::const_iterator i = temps.find(fast_cast<TempNode*>(node));
			if(i == temps.end())
				THROW("Temp node is not registered");

			// напечатать
			glsl << '_' << i->second;
		}
		break;
	case Node::typeTransformed:
		glsl << "v" << fast_cast<TransformedNode*>(node)->GetSemantic();
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
			glsl << (glslVersion == GlslVersions::webgl ? "(texture2D(" : "(texture(");
			glsl << samplerPrefix << sampleNode->GetSamplerNode()->GetSlot() << ", ";
			PrintNode(sampleNode->GetCoordsNode());
			glsl << ')';
			// в GLSL всегда возвращается 4-компонентный вектор, так что возможно, нужно получить компоненты
			int valueSize;
			switch(sampleNode->GetSamplerNode()->GetValueType())
			{
			case DataTypes::_float:
			case DataTypes::_uint:
			case DataTypes::_int:
				valueSize = 1;
				break;
			case DataTypes::_vec2:
			case DataTypes::_uvec2:
			case DataTypes::_ivec2:
				valueSize = 2;
				break;
			case DataTypes::_vec3:
			case DataTypes::_uvec3:
			case DataTypes::_ivec3:
				valueSize = 3;
				break;
			case DataTypes::_vec4:
			case DataTypes::_uvec4:
			case DataTypes::_ivec4:
				valueSize = 4;
				break;
			default:
				THROW("Invalid sampler value type");
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
	case Node::typeFragment:
		{
			FragmentNode* fragmentNode = fast_cast<FragmentNode*>(node);
			switch(glslVersion)
			{
			case GlslVersions::opengl33: glsl << 'r' << fragmentNode->GetTarget(); break;
			case GlslVersions::webgl:
				if(fragmentTargetsCount > 1)
					glsl << "gl_FragData[" << fragmentNode->GetTarget() << ']';
				else
					glsl << "gl_FragColor";
				break;
			}
			glsl << " = (";
			PrintNode(fragmentNode->GetNode());
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
		THROW("Unknown node type");
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
	case OperationNode::operationMul:
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
	case OperationNode::operationScreenToTexture:
		glsl << '(';
		PrintNode(node->GetA());
		glsl << ") * vec2(0.5, 0.5) + vec2(0.5, 0.5)";
		break;
	case OperationNode::operationSaturate:
		glsl << "clamp(";
		PrintNode(node->GetA());
		glsl << ", 0.0, 1.0)";
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
				THROW("Unknown operation type");
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

			return slotA < slotB || (slotA == slotB && a.second->GetOffset() < b.second->GetOffset());
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

		int slot = uniforms[i].first->GetSlot();

		// print header, if needed
		if(supportUniformBuffers)
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
				THROW("Wrong variable offset: should be on 4-byte boundary");

			// если по умолчанию переменная попадёт в неправильное место, делаем пустые переменные, чтобы занять место
			// автоматический сдвиг
			if(currentOffset % sizeof(vec4) + valueSize > sizeof(vec4))
				currentOffset = (currentOffset + sizeof(vec4) - 1) & ~(sizeof(vec4) - 1);
			// оставшийся сдвиг добиваем пустыми переменными (только если uniform буферы)
			if(supportUniformBuffers)
			{
				while(currentOffset < offset)
				{
					int newOffset = (currentOffset + sizeof(vec4)) & ~(sizeof(vec4) - 1);
					if(newOffset > offset)
						newOffset = offset;
					int size = (newOffset - currentOffset) / sizeof(float);
					static const char* dumpTypes[] = { "float", "vec2", "vec3", "vec4" };
					glsl << '\t' << dumpTypes[size - 1] << " dump" << slot << '_' << currentOffset << '_' << size << ";\n";
					currentOffset = newOffset;
				}
			}
			else
				currentOffset = offset;

			// печатаем определение переменной

			glsl << (supportUniformBuffers ? "\t" : "uniform ");
			PrintDataType(valueType);
			// имя переменной
			glsl << ' ' << uniformPrefix << slot << '_' << offset;

			// размер массива
			if(count > 1)
				glsl << '[' << count << ']';
			// если массив, размер элемента должен быть кратен размеру vec4
			if(count > 1 && valueSize % sizeof(vec4))
				THROW("Size of element of array should be multiply of vec4 size");

			// конец переменной
			glsl << ";\n";

			// смещение для следующей переменной
			currentOffset += valueSize * count;
		}

		// ending of buffer
		if(supportUniformBuffers)
			glsl << "};\n";

		i = j;
	}
}

DataType GlslGeneratorInstance::EnforceFloatDataType(DataType dataType)
{
	switch(dataType)
	{
	case DataTypes::_int:
	case DataTypes::_uint:
		dataType = DataTypes::_float;
		break;
	case DataTypes::_ivec2:
	case DataTypes::_uvec2:
		dataType = DataTypes::_vec2;
		break;
	case DataTypes::_ivec3:
	case DataTypes::_uvec3:
		dataType = DataTypes::_vec3;
		break;
	case DataTypes::_ivec4:
	case DataTypes::_uvec4:
		dataType = DataTypes::_vec4;
		break;
	default: break;
	}
	return dataType;
}

bool GlslGeneratorInstance::PrintWebGLConversionToIntegerBegin(DataType dataType)
{
	bool intConversion = false;
	switch(dataType)
	{
	case DataTypes::_int:
	case DataTypes::_uint:
		glsl << "int(";
		intConversion = true;
		break;
	case DataTypes::_ivec2:
	case DataTypes::_uvec2:
		glsl << "ivec2(";
		intConversion = true;
		break;
	case DataTypes::_ivec3:
	case DataTypes::_uvec3:
		glsl << "ivec3(";
		intConversion = true;
		break;
	case DataTypes::_ivec4:
	case DataTypes::_uvec4:
		glsl << "ivec4(";
		intConversion = true;
		break;
	default: break;
	}
	return intConversion;
}

void GlslGeneratorInstance::PrintWebGLConversionToIntegerEnd()
{
	glsl << ')';
}

ptr<ShaderSource> GlslGeneratorInstance::Generate()
{
	// первый проход: зарегистрировать все переменные
	RegisterNode(rootNode);

	// заголовок файла
	switch(glslVersion)
	{
	case GlslVersions::opengl33:
		glsl << "#version 330\n";
		break;
	case GlslVersions::webgl:
		// version numbers in WebGL are not supported
		glsl <<
			"#ifdef GL_ES\n"
			"precision highp float;\n"
			"#endif\n";
		break;
	default:
		THROW("Unknown GLSL version");
	}

	// вывести атрибуты в качестве входных переменных, если это вершинный шейдер
	if(shaderType == ShaderTypes::vertex)
	{
		const char* prefixStr;
		switch(glslVersion)
		{
		case GlslVersions::opengl33: prefixStr = "in "; break;
		case GlslVersions::webgl: prefixStr = "attribute "; break;
		default: THROW("Unknown GLSL version");
		}

		std::sort(attributes.begin(), attributes.end());
		attributes.resize(std::unique(attributes.begin(), attributes.end()) - attributes.begin());
		for(size_t i = 0; i < attributes.size(); ++i)
		{
			ptr<AttributeNode> node = attributes[i];
			glsl << prefixStr;

			DataType valueType = node->GetValueType();

			// hack for lack of supporting integer attributes in WebGL
			// change them to float
			if(glslVersion == GlslVersions::webgl)
				valueType = EnforceFloatDataType(valueType);

			PrintDataType(valueType);
			glsl << " a" << node->GetElementIndex() << ";\n";
		}
	}

	// вывести промежуточные переменные в любом случае
	{
		const char* prefixStr;
		switch(shaderType)
		{
		case ShaderTypes::vertex:
			switch(glslVersion)
			{
			case GlslVersions::opengl33: prefixStr = "out "; break;
			case GlslVersions::webgl: prefixStr = "varying "; break;
			default: THROW("Unknown GLSL version");
			}
			break;
		case ShaderTypes::pixel:
			switch(glslVersion)
			{
			case GlslVersions::opengl33: prefixStr = "in "; break;
			case GlslVersions::webgl: prefixStr = "varying "; break;
			}
			break;
		}

		std::sort(transformed.begin(), transformed.end());
		transformed.resize(std::unique(transformed.begin(), transformed.end()) - transformed.begin());
		for(size_t i = 0; i < transformed.size(); ++i)
		{
			ptr<TransformedNode> node = transformed[i];
			glsl << prefixStr;
			PrintDataType(node->GetValueType());
			glsl << " v" << node->GetSemantic() << ";\n";
		}
	}

	// вывести пиксельные переменные, если это пиксельный шейдер, и это нужно
	if(shaderType == ShaderTypes::pixel && glslVersion == GlslVersions::opengl33)
	{
		for(int i = 0; i < fragmentTargetsCount; ++i)
		{
			glsl << "out ";
			PrintDataType(DataTypes::_vec4);
			glsl << " r" << i << ";\n";
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
		case DataTypes::_float:
		case DataTypes::_vec2:
		case DataTypes::_vec3:
		case DataTypes::_vec4:
			valueTypeStr = "";
			break;
		case DataTypes::_uint:
		case DataTypes::_uvec2:
		case DataTypes::_uvec3:
		case DataTypes::_uvec4:
			valueTypeStr = glslVersion == GlslVersions::webgl ? "i" : "u";
			break;
		case DataTypes::_int:
		case DataTypes::_ivec2:
		case DataTypes::_ivec3:
		case DataTypes::_ivec4:
			valueTypeStr = "i";
			break;
		default:
			THROW("Invalid sampler value type");
		}
		// строка, описывающая размерность
		const char* dimensionStr;
		switch(samplerNode->GetCoordType())
		{
		case SamplerNode::_1D:
			dimensionStr = "1D";
			break;
		case SamplerNode::_2D:
			dimensionStr = "2D";
			break;
		case SamplerNode::_3D:
			dimensionStr = "3D";
			break;
		case SamplerNode::_Cube:
			dimensionStr = "Cube";
			break;
		default:
			THROW("Invalid sampler coord type");
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

	// make list of uniform variable bindings
	GlShaderBindings::UniformBindings uniformBindings;
	if(!supportUniformBuffers)
	{
		uniformBindings.resize(uniforms.size());
		for(size_t i = 0; i < uniforms.size(); ++i)
		{
			ptr<UniformNode> node = uniforms[i].second;
			GlShaderBindings::UniformBinding& uniformBinding = uniformBindings[i];
			uniformBinding.dataType = node->GetValueType();
			uniformBinding.count = node->GetCount();
			uniformBinding.slot = uniforms[i].first->GetSlot();
			uniformBinding.offset = node->GetOffset();
			std::ostringstream ss;
			ss << uniformPrefix << uniformBinding.slot << '_' << uniformBinding.offset;
			uniformBinding.name = ss.str();
		}
	}

	// make list of uniform block bindings
	GlShaderBindings::Bindings uniformBlockBindings;
	if(supportUniformBuffers)
	{
		uniformBlockBindings.resize(uniforms.size());
		for(size_t i = 0; i < uniforms.size(); ++i)
		{
			int slot = uniforms[i].first->GetSlot();
			char name[16];
			sprintf(name, "%s%d", uniformBufferPrefix, slot);
			uniformBlockBindings[i].first = name;
			uniformBlockBindings[i].second = slot;
		}
		// remove duplicates
		std::sort(uniformBlockBindings.begin(), uniformBlockBindings.end());
		uniformBlockBindings.resize(std::unique(uniformBlockBindings.begin(), uniformBlockBindings.end()) - uniformBlockBindings.begin());
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
		int index = attributes[i]->GetElementIndex();
		char name[16];
		sprintf(name, "a%d", index);
		attributeBindings[i].first = name;
		attributeBindings[i].second = index;
	}

	// make a target variables list
	GlShaderBindings::Bindings targetBindings;
	switch(glslVersion)
	{
	case GlslVersions::opengl33:
		targetBindings.resize(fragmentTargetsCount);
		for(int i = 0; i < fragmentTargetsCount; ++i)
		{
			char name[16];
			sprintf(name, "r%d", i);
			targetBindings[i].first = name;
			targetBindings[i].second = i;
		}
		break;
	case GlslVersions::webgl:
		// no bindings needed, because of use of gl_FragColor/gl_FragData
		break;
	}

	return NEW(GlslSource(
		Strings::String2File(glsl.str()),
		NEW(GlShaderBindings(uniformBindings, uniformBlockBindings, samplerBindings, attributeBindings, targetBindings))
	));
}

END_INANITY_SHADERS
