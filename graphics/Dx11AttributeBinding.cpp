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
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			default:
				THROW("Unsupported layout data type for float data type");
			}
			break;
		case DataTypes::_vec2:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			default:
				THROW("Unsupported layout data type for vec2 data type");
			}
			break;
		case DataTypes::_vec3:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			default:
				THROW("Unsupported layout data type for vec3 data type");
			}
			break;
		case DataTypes::_vec4:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			default:
				THROW("Unsupported layout data type for vec4 data type");
			}
			break;
		case DataTypes::_mat4x4:
			THROW("Matrices can't be used in attributes");
		case DataTypes::_uint:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case LayoutDataType::Uint16:
				format = DXGI_FORMAT_R16_UINT;
				break;
			case LayoutDataType::Uint8:
				format = DXGI_FORMAT_R8_UINT;
				break;
			default:
				THROW("Unsupported layout data type for uint data type");
			}
			break;
		case DataTypes::_uvec2:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case LayoutDataType::Uint16:
				format = DXGI_FORMAT_R16G16_UINT;
				break;
			case LayoutDataType::Uint8:
				format = DXGI_FORMAT_R8G8_UINT;
				break;
			default:
				THROW("Unsupported layout data type for uvec2 data type");
			}
			break;
		case DataTypes::_uvec3:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			// there is no DXGI_FORMAT_R16G16B16_UINT or DXGI_FORMAT_R8G8B8_UINT
			default:
				THROW("Unsupported layout data type for uvec3 data type");
			}
			break;
		case DataTypes::_uvec4:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case LayoutDataType::Uint16:
				format = DXGI_FORMAT_R16G16B16A16_UINT;
				break;
			case LayoutDataType::Uint8:
				format = DXGI_FORMAT_R8G8B8A8_UINT;
				break;
			default:
				THROW("Unsupported layout data type for uvec4 data type");
			}
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

ComPointer<ID3D11InputLayout> Dx11AttributeBinding::CreateInputLayout(ID3D11Device* deviceInterface, Dx11VertexShader* vertexShader)
{
	// получить код шейдера
	ptr<File> code = vertexShader->GetCode();

	// создать разметку
	ComPointer<ID3D11InputLayout> inputLayout;
	if(FAILED(deviceInterface->CreateInputLayout(
		&*descs.begin(), (UINT)descs.size(),
		code->GetData(), code->GetSize(), &inputLayout
	)))
		THROW("Can't create input layout");

	return inputLayout;
}

END_INANITY_GRAPHICS
