#include "SlGeneratorInstance.hpp"
#include "AttributeNode.hpp"
#include "UniformNode.hpp"
#include "SamplerNode.hpp"
#include "ReadUniformNode.hpp"
#include "IndexUniformArrayNode.hpp"
#include "TransformedNode.hpp"
#include "InterpolateNode.hpp"
#include "SequenceNode.hpp"
#include "SwizzleNode.hpp"
#include "OperationNode.hpp"
#include "ActionNode.hpp"
#include "SampleNode.hpp"
#include "FragmentNode.hpp"
#include "CastNode.hpp"
#include "UniformGroup.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

SlGeneratorInstance::SlGeneratorInstance(ptr<Node> rootNode, ShaderType shaderType) :
	rootNode(rootNode),
	shaderType(shaderType),
	fragmentTargetsCount(0)
{}

void SlGeneratorInstance::RegisterNode(Node* node)
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

			attributes.push_back(fast_cast<AttributeNode*>(node));
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

			ValueNode* lodNode = sampleNode->GetLodNode();
			if(lodNode)
				RegisterNode(lodNode);

			ValueNode* biasNode = sampleNode->GetBiasNode();
			if(biasNode)
				RegisterNode(biasNode);

			ValueNode* gradXNode = sampleNode->GetGradXNode();
			if(gradXNode)
				RegisterNode(gradXNode);
			ValueNode* gradYNode = sampleNode->GetGradYNode();
			if(gradYNode)
				RegisterNode(gradYNode);

			ValueNode* offsetNode = sampleNode->GetOffsetNode();
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

void SlGeneratorInstance::PrintNode(ValueNode* node)
{
	THROW_ASSERT(nodeInitIndices.count(node) > 0);
	text << '_' << nodeInitIndices[node];
}

void SlGeneratorInstance::PrintNodeInitVar(int nodeIndex)
{
	text << '\t';
	PrintDataType(fast_cast<ValueNode*>(nodeInits[nodeIndex])->GetValueType());
	text << " _" << nodeIndex;
}

void SlGeneratorInstance::PrintNodeInitBegin(int nodeIndex)
{
	PrintNodeInitVar(nodeIndex);
	text << " = ";
}

void SlGeneratorInstance::PrintNodeInitEnd()
{
	text << ";\n";
}

END_INANITY_SHADERS
