#include "Instancer.hpp"
#include "AttributeNode.hpp"
#include "OperationNode.hpp"
#include "../UniformBuffer.hpp"
#include "UniformGroup.hpp"
#include "Uniform.ipp"
#include "../Device.hpp"
#include "../Context.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.hpp"
#include "../VertexLayout.hpp"
#include "../VertexShader.hpp"
#include "../PixelShader.hpp"
#include "../BlendState.hpp"
#include "../VertexLayoutElement.hpp"
#include "../AttributeLayout.hpp"
#include "../AttributeLayoutSlot.hpp"
#include "../AttributeLayoutElement.hpp"
#include "../../MemoryFile.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

Instancer::Instancer(ptr<Device> device, int maximumInstancesCount, ptr<AttributeLayout> attributeLayout)
{
	BEGIN_TRY();

	// get device capabilities
	Device::Caps caps = device->GetCaps();
	// if hardware instancing is supported
	if(caps.flags & Device::Caps::drawInstancing)
	{
		// the easiest way
		mode = modeDrawInstancing;

		nodeInstanceID = NEW(OperationNode(OperationNode::operationGetInstanceID));
	}
	else if(caps.flags & Device::Caps::attributeInstancing)
	{
		// instance numbers will be in a vertex buffer
		mode = modeAttributeInstancing;

		// create layouts
		ptr<VertexLayout> vl = NEW(VertexLayout(sizeof(uint)));
		ptr<VertexLayoutElement> vle = vl->AddElement(DataTypes::_uint, 0);
		als = attributeLayout->AddSlot(1);
		nodeInstanceID = NEW(AttributeNode(attributeLayout->AddElement(als, vle)));

		// create vertex buffer
		ptr<MemoryFile> verticesFile = NEW(MemoryFile(maximumInstancesCount * sizeof(uint)));
		uint* verticesData = (uint*)verticesFile->GetData();
		for(int i = 0; i < maximumInstancesCount; ++i)
			verticesData[i] = i;
		vb = device->CreateStaticVertexBuffer(verticesFile, vl);
	}
	else
	{
		// fallback mode
		mode = modeFallback;

		ugInstanceID = NEW(UniformGroup(uniformBufferSlot));
		nodeInstanceID = ugInstanceID->AddUniform<uint>().GetNode();
		ugInstanceID->Finalize(device);
	}

	END_TRY("Can't create instancer");
}

Value<uint> Instancer::GetInstanceID() const
{
	return nodeInstanceID;
}

void Instancer::Draw(Context* context, int instancesCount)
{
	switch(mode)
	{
	case modeDrawInstancing:
		context->DrawInstanced(instancesCount);
		break;
	case modeAttributeInstancing:
		{
			Context::LetVertexBuffer lvb(context, &*als, vb);
			context->DrawInstanced(instancesCount);
		}
		break;
	case modeFallback:
		{
			Context::LetUniformBuffer lub(context, ugInstanceID);
			Uniform<uint> u(nodeInstanceID.StaticCast<UniformNode>());
			for(int i = 0; i < instancesCount; ++i)
			{
				u.SetValue(i);
				ugInstanceID->Upload(context);
				context->Draw();
			}
		}
		break;
	}
}

END_INANITY_SHADERS
