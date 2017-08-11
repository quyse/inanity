#include "GlslGeneratorInstance.hpp"
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
#include "DualFragmentNode.hpp"
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
	SlGeneratorInstance(rootNode, shaderType),
	glslVersion(glslVersion),
	supportUniformBuffers(supportUniformBuffers)
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
	case DataTypes::_bool:			name = "bool";			break;
	case DataTypes::_bvec2:			name = "bvec2";			break;
	case DataTypes::_bvec3:			name = "bvec3";			break;
	case DataTypes::_bvec4:			name = "bvec4";			break;
	default:
		THROW("Unknown data type");
	}
	text << name;
}

void GlslGeneratorInstance::PrintUniform(UniformNode* uniformNode)
{
	text << uniformPrefix << uniformNode->GetGroup()->GetSlot() << '_' << uniformNode->GetOffset();
}

void GlslGeneratorInstance::PrintNodeInit(size_t nodeIndex)
{
	Node* node = nodeInits[nodeIndex];

	switch(node->GetType())
	{
	case Node::typeFloatConst:
		PrintNodeInitBegin(nodeIndex);
		text << std::fixed << std::setprecision(10) << fast_cast<FloatConstNode*>(node)->GetValue();// << 'f';
		PrintNodeInitEnd();
		break;
	case Node::typeIntConst:
		PrintNodeInitBegin(nodeIndex);
		text << fast_cast<IntConstNode*>(node)->GetValue();
		PrintNodeInitEnd();
		break;
	case Node::typeAttribute:
		{
			AttributeNode* attributeNode = fast_cast<AttributeNode*>(node);
			PrintNodeInitBegin(nodeIndex);
			// WebGL hack for integer attributes
			// (only for case of using another hack - changing integer attributes to float)
			bool intConversion = false;
			if(glslVersion == GlslVersions::webgl)
				intConversion = PrintWebGLConversionToIntegerBegin(attributeNode->GetValueType());

			text << "a" << attributeNode->GetElementIndex();

			if(intConversion)
				PrintWebGLConversionToIntegerEnd();
			PrintNodeInitEnd();
		}
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
		text << "v" << fast_cast<TransformedNode*>(node)->GetSemantic();
		PrintNodeInitEnd();
		break;
	case Node::typeInterpolate:
		{
			InterpolateNode* interpolateNode = fast_cast<InterpolateNode*>(node);
			text << "\tv" << interpolateNode->GetSemantic() << " = ";
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

			ValueNode* coordsNode = sampleNode->GetCoordsNode();
			ValueNode* offsetNode = sampleNode->GetOffsetNode();
			ValueNode* lodNode = sampleNode->GetLodNode();
			ValueNode* biasNode = sampleNode->GetBiasNode();
			ValueNode* gradXNode = sampleNode->GetGradXNode();
			ValueNode* gradYNode = sampleNode->GetGradYNode();

			auto printOffsetNode = [&]()
			{
				// offset node is required to be constant expression,
				// but AMD driver requires it also to be compile-time expression
				// so HACK: print value inline, only for known node
				// PrintNode(offsetNode);
				if(offsetNode->GetType() != Node::typeOperation)
					THROW("wrong offset node");
				OperationNode* offsetOperationNode = fast_cast<OperationNode*>(offsetNode);
				if(
					offsetOperationNode->GetOperation() != OperationNode::operationInt11to2 ||
					offsetOperationNode->GetA()->GetType() != Node::typeIntConst ||
					offsetOperationNode->GetB()->GetType() != Node::typeIntConst
					)
					THROW("wrong offset node");
				text << "ivec2("
					<< offsetOperationNode->GetA().FastCast<IntConstNode>()->GetValue() << ", "
					<< offsetOperationNode->GetB().FastCast<IntConstNode>()->GetValue() << ")";
			};

			if(lodNode)
			{
				text << "textureLod";
				if(offsetNode)
					text << "Offset";
				text << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				text << ", ";
				PrintNode(lodNode);
				if(offsetNode)
				{
					text << ", ";
					printOffsetNode();
				}
			}
			else if(biasNode)
			{
				text << "texture";
				if(offsetNode)
					text << "Offset";
				text << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				if(offsetNode)
				{
					text << ", ";
					printOffsetNode();
				}
				text << ", ";
				PrintNode(biasNode);
			}
			else if(gradXNode && gradYNode)
			{
				text << "textureGrad";
				if(offsetNode)
					text << "Offset";
				text << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				text << ", ";
				PrintNode(gradXNode);
				text << ", ";
				PrintNode(gradYNode);
				if(offsetNode)
				{
					text << ", ";
					printOffsetNode();
				}
			}
			else
			{
				text << (glslVersion == GlslVersions::webgl ? "texture2D" : "texture");
				if(offsetNode && glslVersion != GlslVersions::webgl)
					text << "Offset";
				text << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				if(offsetNode && glslVersion != GlslVersions::webgl)
				{
					text << ", ";
					printOffsetNode();
				}
			}

			// close sample call
			text << ')';

			// sample always returns four-component vector, so make some swizzle if needed
			int valueSize = GetVectorDataTypeDimensions(sampleNode->GetValueType());
			if(valueSize < 4)
			{
				text << '.';
				for(int i = 0; i < valueSize; ++i)
					text << "xyzw"[i];
			}

			PrintNodeInitEnd();
		}
		break;
	case Node::typeFragment:
		{
			FragmentNode* fragmentNode = fast_cast<FragmentNode*>(node);
			switch(glslVersion)
			{
			case GlslVersions::opengl33: text << "\tr" << fragmentNode->GetTarget(); break;
			case GlslVersions::webgl:
				if(fragmentTargetsCount > 1)
					text << "gl_FragData[" << fragmentNode->GetTarget() << ']';
				else
					text << "gl_FragColor";
				break;
			}
			text << " = ";
			PrintNode(fragmentNode->GetNode());
			PrintNodeInitEnd();
		}
		break;
	case Node::typeDualFragment:
		{
			DualFragmentNode* dualFragmentNode = fast_cast<DualFragmentNode*>(node);
			switch(glslVersion)
			{
			case GlslVersions::opengl33:
				text << "\tr0 = ";
				PrintNode(dualFragmentNode->GetNode0());
				PrintNodeInitEnd();
				text << "\tr1 = ";
				PrintNode(dualFragmentNode->GetNode1());
				PrintNodeInitEnd();
				break;
			case GlslVersions::webgl:
				text << "\tgl_FragData[0] = ";
				PrintNode(dualFragmentNode->GetNode0());
				PrintNodeInitEnd();
				text << "\tgl_FragData[1] = ";
				PrintNode(dualFragmentNode->GetNode1());
				PrintNodeInitEnd();
				break;
			}
		}
		break;
	case Node::typeCast:
		{
			PrintNodeInitBegin(nodeIndex);
			CastNode* castNode = fast_cast<CastNode*>(node);
			PrintDataType(castNode->GetValueType());
			text << '(';
			PrintNode(castNode->GetA());
			text << ')';
			PrintNodeInitEnd();
		}
		break;
	default:
		THROW("Unknown node type");
	}
}

void GlslGeneratorInstance::PrintOperationNodeInit(OperationNode* node)
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
	case OperationNode::operationMul:
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
		text << "uint(gl_InstanceID)";
		break;
	case OperationNode::operationScreenToTexture:
		PrintNode(node->GetA());
		text << " * vec2(0.5, 0.5) + vec2(0.5, 0.5)";
		break;
	case OperationNode::operationSaturate:
		text << "clamp(";
		PrintNode(node->GetA());
		text << ", 0.0, 1.0)";
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
				OP(Int11to2, ivec2);
				OP(Dot, dot);
				OP(Cross, cross);
				OP(Mul, mul);
				OP(Length, length);
				OP(Normalize, normalize);
				OP(Lerp, mix);
				OP(Pow, pow);
				OP(Min, min);
				OP(Max, max);
				OP(Abs, abs);
				OP(Sqrt, sqrt);
				OP(Sin, sin);
				OP(Atan2, atan);
				OP(Cos, cos);
				OP(Exp, exp);
				OP(Exp2, exp2);
				OP(Log, log);
				OP(Ddx, dFdx);
				OP(Ddy, dFdy);
				OP(Floor, floor);
				OP(Ceil, ceil);
				OP(Mod, mod);
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

void GlslGeneratorInstance::PrintActionNodeInit(ActionNode* node)
{
	switch(node->GetAction())
	{
	case ActionNode::actionSetPosition:
		text << "\tgl_Position = ";
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

void GlslGeneratorInstance::PrintUniforms()
{
	// uniform-буферы и переменные
	// отсортировать их
	struct Sorter
	{
		bool operator()(const std::pair<UniformGroup*, UniformNode*>& a, const std::pair<UniformGroup*, UniformNode*>& b) const
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
			text << "layout(std140) uniform " << uniformBufferPrefix << slot << "\n{\n";

		// текущее смещение от начала буфера
		int currentOffset = 0;

		// вывести переменные
		for(size_t k = i; k < j; ++k)
		{
			UniformNode* uniformNode = uniforms[k].second;
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
					text << '\t' << dumpTypes[size - 1] << " dump" << slot << '_' << currentOffset << '_' << size << ";\n";
					currentOffset = newOffset;
				}
			}
			else
				currentOffset = offset;

			// печатаем определение переменной

			text << (supportUniformBuffers ? "\t" : "uniform ");
			PrintDataType(valueType);
			// имя переменной
			text << ' ' << uniformPrefix << slot << '_' << offset;

			// размер массива
			if(count > 1)
				text << '[' << count << ']';
			// если массив, размер элемента должен быть кратен размеру vec4
			if(count > 1 && valueSize % sizeof(vec4))
				THROW("Size of element of array should be multiply of vec4 size");

			// конец переменной
			text << ";\n";

			// смещение для следующей переменной
			currentOffset += valueSize * count;
		}

		// ending of buffer
		if(supportUniformBuffers)
			text << "};\n";

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
		text << "int(";
		intConversion = true;
		break;
	case DataTypes::_ivec2:
	case DataTypes::_uvec2:
		text << "ivec2(";
		intConversion = true;
		break;
	case DataTypes::_ivec3:
	case DataTypes::_uvec3:
		text << "ivec3(";
		intConversion = true;
		break;
	case DataTypes::_ivec4:
	case DataTypes::_uvec4:
		text << "ivec4(";
		intConversion = true;
		break;
	default: break;
	}
	return intConversion;
}

void GlslGeneratorInstance::PrintWebGLConversionToIntegerEnd()
{
	text << ')';
}

ptr<ShaderSource> GlslGeneratorInstance::Generate()
{
	// first stage: register all nodes
	RegisterNode(rootNode);

	// заголовок файла
	switch(glslVersion)
	{
	case GlslVersions::opengl33:
		text << "#version 330\n";
		break;
	case GlslVersions::webgl:
		// version numbers in WebGL are not supported
		text <<
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
		for(size_t i = 0; i < attributes.size(); ++i)
		{
			AttributeNode* node = attributes[i];
			text << prefixStr;

			DataType valueType = node->GetValueType();

			// hack for lack of supporting integer attributes in WebGL
			// change them to float
			if(glslVersion == GlslVersions::webgl)
				valueType = EnforceFloatDataType(valueType);

			PrintDataType(valueType);
			text << " a" << node->GetElementIndex() << ";\n";
		}
	}

	// print transformed nodes
	if(!transformedNodes.empty())
	{
		if(shaderType != ShaderTypes::pixel)
			THROW("Only pixel shader may have transformed nodes");
		const char* prefixStr;
		switch(glslVersion)
		{
		case GlslVersions::opengl33: prefixStr = "in "; break;
		case GlslVersions::webgl: prefixStr = "varying "; break;
		default: THROW("Unknown GLSL version");
		}

		for(size_t i = 0; i < transformedNodes.size(); ++i)
		{
			TransformedNode* node = transformedNodes[i];
			text << prefixStr;
			PrintDataType(node->GetValueType());
			text << " v" << node->GetSemantic() << ";\n";
		}
	}

	// print interpolate nodes
	if(!interpolateNodes.empty())
	{
		if(shaderType != ShaderTypes::vertex)
			THROW("Only vertex shader may have interpolate nodes");
		const char* prefixStr;
		switch(glslVersion)
		{
		case GlslVersions::opengl33: prefixStr = "out "; break;
		case GlslVersions::webgl: prefixStr = "varying "; break;
		default: THROW("Unknown GLSL version");
		}

		for(size_t i = 0; i < interpolateNodes.size(); ++i)
		{
			InterpolateNode* node = interpolateNodes[i];
			text << prefixStr;
			PrintDataType(node->GetValueType());
			text << " v" << node->GetSemantic() << ";\n";
		}
	}

	// вывести пиксельные переменные, если это пиксельный шейдер, и это нужно
	if(shaderType == ShaderTypes::pixel && glslVersion == GlslVersions::opengl33)
	{
		for(int i = 0; i < fragmentTargetsCount; ++i)
		{
			text << "out ";
			PrintDataType(DataTypes::_vec4);
			text << " r" << i << ";\n";
		}
	}

	// print uniforms
	PrintUniforms();

	// samplers
	for(size_t i = 0; i < samplers.size(); ++i)
	{
		SamplerNode* samplerNode = samplers[i];
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
		text << "uniform " << valueTypeStr << "sampler" << dimensionStr << ' ' << samplerPrefix << slot << ";\n";
	}

	//** заголовок функции шейдера

	text << "void main()\n{\n";

	// shader code
	for(size_t i = 0; i < nodeInits.size(); ++i)
		PrintNodeInit(i);

	// завершение шейдера
	text << "}\n";

	// make list of uniform variable bindings
	GlShaderBindings::UniformBindings uniformBindings;
	if(!supportUniformBuffers)
	{
		uniformBindings.resize(uniforms.size());
		for(size_t i = 0; i < uniforms.size(); ++i)
		{
			UniformNode* node = uniforms[i].second;
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
		Strings::String2File(text.str()),
		NEW(GlShaderBindings(uniformBindings, uniformBlockBindings, samplerBindings, attributeBindings, targetBindings, dualFragmentTarget))
	));
}

END_INANITY_SHADERS
