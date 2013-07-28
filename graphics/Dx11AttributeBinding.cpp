#include "Dx11AttributeBinding.hpp"
#include "Dx11System.hpp"
#include "Dx11VertexShader.hpp"
#include "AttributeLayout.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

void Dx11AttributeBinding::Initialize(ptr<AttributeLayout> layout)
{
	const AttributeLayout::Elements& elements = layout->GetElements();
	const AttributeLayout::Slots& slots = layout->GetSlots();

	descs.resize(elements.size());
	semanticNames.resize(elements.size());

	// заполнить их
	for(size_t i = 0; i < elements.size(); ++i)
	{
		const AttributeLayout::Element& element = elements[i];

		// выбрать формат элемента
		DXGI_FORMAT format;
		switch(element.dataType)
		{
		case DataTypes::_float:
			format = DXGI_FORMAT_R32_FLOAT;
			break;
		case DataTypes::_vec2:
			format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case DataTypes::_vec3:
			format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case DataTypes::_vec4:
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case DataTypes::_mat4x4:
			THROW("Matrices can't be used in attributes");
		case DataTypes::_uint:
			format = DXGI_FORMAT_R32_UINT;
			break;
		case DataTypes::_uvec2:
			format = DXGI_FORMAT_R32G32_UINT;
			break;
		case DataTypes::_uvec3:
			format = DXGI_FORMAT_R32G32B32_UINT;
			break;
		case DataTypes::_uvec4:
			format = DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		default:
			THROW("Unknown element type");
		}

		// получить информацию о слоте
		const AttributeLayout::Slot& slot = slots[element.slot];

		String& semanticName = semanticNames[i];
		semanticName = Dx11System::GetSemanticString(i);

		D3D11_INPUT_ELEMENT_DESC& desc = descs[i];
		desc.SemanticName = semanticName.c_str();
		desc.SemanticIndex = 0;
		desc.Format = format;
		desc.InputSlot = element.slot;
		desc.AlignedByteOffset = element.offset;
		desc.InputSlotClass = slot.divisor ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = slot.divisor;
	}
}

ID3D11InputLayout* Dx11AttributeBinding::CreateInputLayout(ID3D11Device* deviceInterface, Dx11VertexShader* vertexShader)
{
	// получить код шейдера
	ptr<File> code = vertexShader->GetCode();

	// создать разметку
	ID3D11InputLayout* inputLayoutInterface;
	if(FAILED(deviceInterface->CreateInputLayout(
		&*descs.begin(), (UINT)descs.size(),
		code->GetData(), code->GetSize(), &inputLayoutInterface
	)))
		THROW("Can't create input layout");

	return inputLayoutInterface;
}

END_INANITY_GRAPHICS
