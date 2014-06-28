#include "GlslGeneratorInstance.hpp"
#include "Node.hpp"
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
	case DataTypes::_bool:			name = "bool";			break;
	case DataTypes::_bvec2:			name = "bvec2";			break;
	case DataTypes::_bvec3:			name = "bvec3";			break;
	case DataTypes::_bvec4:			name = "bvec4";			break;
	default:
		THROW("Unknown data type");
	}
	glsl << name;
}

void GlslGeneratorInstance::PrintUniform(UniformNode* uniformNode)
{
	glsl << uniformPrefix << uniformNode->GetGroup()->GetSlot() << '_' << uniformNode->GetOffset();
}

void GlslGeneratorInstance::RegisterNode(Node* node)
{
	// if registration of node already began
	if(registeredNodes.find(node) != registeredNodes.end())
	{
		// if node is not registered yet
		if(nodeInitIndices.find(node) == nodeInitIndices.end())
			// than it's a loop
			THROW("Node cyclic dependency");
		// else it's ok, it's just another use of node
		return;
	}
	// register node
	registeredNodes.insert(node);

	switch(node->GetType())
	{
	case Node::typeFloatConst:
	case Node::typeIntConst:
		break;
	case Node::typeAttribute:
		{
			if(shaderType != ShaderTypes::vertex)
				THROW("Only vertex shader can have attribute nodes");

			ptr<AttributeNode> attributeNode = fast_cast<AttributeNode*>(node);
			attributes.push_back(attributeNode);
		}
		break;
	case Node::typeUniform:
		{
			UniformNode* uniformNode = fast_cast<UniformNode*>(node);
			uniforms.push_back(std::make_pair(uniformNode->GetGroup(), uniformNode));
		}
		break;
	case Node::typeSampler:
		samplers.push_back(fast_cast<SamplerNode*>(node));
		break;
	case Node::typeReadUniform:
		RegisterNode(fast_cast<ReadUniformNode*>(node)->GetUniformNode());
		break;
	case Node::typeIndexUniformArray:
		{
			IndexUniformArrayNode* indexUniformArrayNode = fast_cast<IndexUniformArrayNode*>(node);
			RegisterNode(indexUniformArrayNode->GetUniformNode());
			RegisterNode(indexUniformArrayNode->GetIndexNode());
		}
		break;
	case Node::typeTransformed:
		transformedNodes.push_back(fast_cast<TransformedNode*>(node));
		break;
	case Node::typeInterpolate:
		{
			InterpolateNode* interpolateNode = fast_cast<InterpolateNode*>(node);
			interpolateNodes.push_back(interpolateNode);
			RegisterNode(interpolateNode->GetNode());
		}
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
	case Node::typeAction:
		{
			ActionNode* actionNode = fast_cast<ActionNode*>(node);
			int argumentsCount = actionNode->GetArgumentsCount();
			for(int i = 0; i < argumentsCount; ++i)
				RegisterNode(actionNode->GetArgument(i));
		}
		break;
	case Node::typeSample:
		{
			SampleNode* sampleNode = fast_cast<SampleNode*>(node);

			RegisterNode(sampleNode->GetSamplerNode());
			RegisterNode(sampleNode->GetCoordsNode());

			ptr<ValueNode> lodNode = sampleNode->GetLodNode();
			if(lodNode)
				RegisterNode(lodNode);

			ptr<ValueNode> biasNode = sampleNode->GetBiasNode();
			if(biasNode)
				RegisterNode(biasNode);

			ptr<ValueNode> gradXNode = sampleNode->GetGradXNode();
			if(gradXNode)
				RegisterNode(gradXNode);
			ptr<ValueNode> gradYNode = sampleNode->GetGradYNode();
			if(gradYNode)
				RegisterNode(gradYNode);

			ptr<ValueNode> offsetNode = sampleNode->GetOffsetNode();
			if(offsetNode)
				RegisterNode(offsetNode);
		}
		break;
	case Node::typeFragment:
		{
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

	// add initialization of node
	THROW_ASSERT(nodeInitIndices.find(node) == nodeInitIndices.end());
	nodeInitIndices[node] = (int)nodeInits.size();
	nodeInits.push_back(node);
}

void GlslGeneratorInstance::PrintNodeInitVar(int nodeIndex)
{
	glsl << '\t';
	PrintDataType(fast_cast<ValueNode*>(nodeInits[nodeIndex])->GetValueType());
	glsl << " _" << nodeIndex;
}

void GlslGeneratorInstance::PrintNodeInitBegin(int nodeIndex)
{
	PrintNodeInitVar(nodeIndex);
	glsl << " = ";
}

void GlslGeneratorInstance::PrintNodeInitEnd()
{
	glsl << ";\n";
}

void GlslGeneratorInstance::PrintNodeInit(int nodeIndex)
{
	Node* node = nodeInits[nodeIndex];

	switch(node->GetType())
	{
	case Node::typeFloatConst:
		PrintNodeInitBegin(nodeIndex);
		glsl << std::fixed << std::setprecision(10) << fast_cast<FloatConstNode*>(node)->GetValue();// << 'f';
		PrintNodeInitEnd();
		break;
	case Node::typeIntConst:
		PrintNodeInitBegin(nodeIndex);
		glsl << fast_cast<IntConstNode*>(node)->GetValue();
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

			glsl << "a" << attributeNode->GetElementIndex();

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
			glsl << '[';
			PrintNode(indexUniformArrayNode->GetIndexNode());
			glsl << ']';
			PrintNodeInitEnd();
		}
		break;
	case Node::typeTransformed:
		PrintNodeInitBegin(nodeIndex);
		glsl << "v" << fast_cast<TransformedNode*>(node)->GetSemantic();
		PrintNodeInitEnd();
		break;
	case Node::typeInterpolate:
		{
			InterpolateNode* interpolateNode = fast_cast<InterpolateNode*>(node);
			glsl << "\tv" << interpolateNode->GetSemantic() << " = ";
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
			glsl << '.' << swizzleNode->GetMap();
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
			SampleNode* sampleNode = fast_cast<SampleNode*>(node);

			int slot = sampleNode->GetSamplerNode()->GetSlot();

			ptr<ValueNode> coordsNode = sampleNode->GetCoordsNode();
			ptr<ValueNode> offsetNode = sampleNode->GetOffsetNode();
			ptr<ValueNode> lodNode = sampleNode->GetLodNode();
			ptr<ValueNode> biasNode = sampleNode->GetBiasNode();
			ptr<ValueNode> gradXNode = sampleNode->GetGradXNode();
			ptr<ValueNode> gradYNode = sampleNode->GetGradYNode();

			PrintNodeInitBegin(nodeIndex);

			if(lodNode)
			{
				glsl << "textureLod";
				if(offsetNode)
					glsl << "Offset";
				glsl << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				glsl << ", ";
				PrintNode(lodNode);
				if(offsetNode)
				{
					glsl << ", ";
					PrintNode(offsetNode);
				}
			}
			else if(biasNode)
			{
				glsl << "texture";
				if(offsetNode)
					glsl << "Offset";
				glsl << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				if(offsetNode)
				{
					glsl << ", ";
					PrintNode(offsetNode);
				}
				glsl << ", ";
				PrintNode(biasNode);
			}
			else if(gradXNode && gradYNode)
			{
				glsl << "textureGrad";
				if(offsetNode)
					glsl << "Offset";
				glsl << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				glsl << ", ";
				PrintNode(gradXNode);
				glsl << ", ";
				PrintNode(gradYNode);
				if(offsetNode)
				{
					glsl << ", ";
					PrintNode(offsetNode);
				}
			}
			else
			{
				glsl << (glslVersion == GlslVersions::webgl ? "texture2D" : "texture");
				if(offsetNode)
					glsl << "Offset";
				glsl << '(' << samplerPrefix << slot << ", ";
				PrintNode(coordsNode);
				if(offsetNode)
				{
					glsl << ", ";
					PrintNode(offsetNode);
				}
			}

			// close sample call
			glsl << ')';

			// sample always returns four-component vector, so make some swizzle if needed
			int valueSize = GetVectorDataTypeDimensions(sampleNode->GetValueType());
			if(valueSize < 4)
			{
				glsl << '.';
				for(int i = 0; i < valueSize; ++i)
					glsl << "xyzw"[i];
			}

			PrintNodeInitEnd();
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
			glsl << " = ";
			PrintNode(fragmentNode->GetNode());
			glsl << ";\n";
		}
		break;
	case Node::typeCast:
		{
			PrintNodeInitBegin(nodeIndex);
			CastNode* castNode = fast_cast<CastNode*>(node);
			PrintDataType(castNode->GetValueType());
			glsl << '(';
			PrintNode(castNode->GetA());
			glsl << ')';
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
		glsl << '-';
		PrintNode(node->GetA());
		break;
	case OperationNode::operationAdd:
		PrintNode(node->GetA());
		glsl << " + ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationSubtract:
		PrintNode(node->GetA());
		glsl << " - ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationMultiply:
	case OperationNode::operationMul:
		PrintNode(node->GetA());
		glsl << " * ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationDivide:
		PrintNode(node->GetA());
		glsl << " / ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationLess:
		PrintNode(node->GetA());
		glsl << " < ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationLessEqual:
		PrintNode(node->GetA());
		glsl << " <= ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationEqual:
		PrintNode(node->GetA());
		glsl << " == ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationNotEqual:
		PrintNode(node->GetA());
		glsl << " != ";
		PrintNode(node->GetB());
		break;
	case OperationNode::operationGetInstanceID:
		glsl << "uint(gl_InstanceID)";
		break;
	case OperationNode::operationScreenToTexture:
		PrintNode(node->GetA());
		glsl << " * vec2(0.5, 0.5) + vec2(0.5, 0.5)";
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
				OP(Lerp, mix);
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
				OP(Floor, floor);
				OP(Ceil, ceil);
				OP(Mod, mod);
#undef OP
			default:
				THROW("Invalid operation type");
			}

			// вывести
			glsl << name << '(';
			int argumentsCount = node->GetArgumentsCount();
			for(int i = 0; i < argumentsCount; ++i)
			{
				if(i)
					glsl << ", ";
				PrintNode(node->GetArgument(i));
			}
			glsl << ')';
		}
	}
}

void GlslGeneratorInstance::PrintActionNodeInit(ActionNode* node)
{
	switch(node->GetAction())
	{
	case ActionNode::actionSetPosition:
		glsl << "\tgl_Position = ";
		PrintNode(node->GetA());
		PrintNodeInitEnd();
		break;
	case ActionNode::actionClip:
		glsl << "\tclip(";
		PrintNode(node->GetA());
		glsl << ')';
		PrintNodeInitEnd();
		break;
	default:
		THROW("Invalid action type");
	}
}

void GlslGeneratorInstance::PrintNode(ValueNode* node)
{
	THROW_ASSERT(nodeInitIndices.count(node) > 0);
	glsl << '_' << nodeInitIndices[node];
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
	// first stage: register all nodes
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
			glsl << prefixStr;
			PrintDataType(node->GetValueType());
			glsl << " v" << node->GetSemantic() << ";\n";
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

	// print uniforms
	PrintUniforms();

	// samplers
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

	// shader code
	for(int i = 0; i < (int)nodeInits.size(); ++i)
		PrintNodeInit(i);

	// завершение шейдера
	glsl << "}\n";

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
