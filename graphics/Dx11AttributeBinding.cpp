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
		case DataTypes::_uint:
		case DataTypes::_int:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case LayoutDataType::Uint16:
				format = DXGI_FORMAT_R16_UINT;
				break;
			case LayoutDataType::Uint8:
				format = DXGI_FORMAT_R8_UINT;
				break;
			case LayoutDataTypes::Int32:
				format = DXGI_FORMAT_R32_SINT;
				break;
			case LayoutDataType::Int16:
				format = DXGI_FORMAT_R16_SINT;
				break;
			case LayoutDataType::Int8:
				format = DXGI_FORMAT_R8_SINT;
				break;
			default:
				THROW("Unsupported layout data type for scalar data type");
			}
			break;
		case DataTypes::_vec2:
		case DataTypes::_uvec2:
		case DataTypes::_ivec2:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case LayoutDataType::Uint16:
				format = DXGI_FORMAT_R16G16_UINT;
				break;
			case LayoutDataType::Uint8:
				format = DXGI_FORMAT_R8G8_UINT;
				break;
			case LayoutDataType::Int32:
				format = DXGI_FORMAT_R32G32_SINT;
				break;
			case LayoutDataType::Int16:
				format = DXGI_FORMAT_R16G16_SINT;
				break;
			case LayoutDataType::Int8:
				format = DXGI_FORMAT_R8G8_SINT;
				break;
			default:
				THROW("Unsupported layout data type for 2-vector data type");
			}
			break;
		case DataTypes::_vec3:
		case DataTypes::_uvec3:
		case DataTypes::_ivec3:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case LayoutDataType::Int32:
				format = DXGI_FORMAT_R32G32B32_SINT;
				break;
			// there are no 16/8 bit three element formats
			default:
				THROW("Unsupported layout data type for 3-vector data type");
			}
			break;
		case DataTypes::_vec4:
		case DataTypes::_uvec4:
		case DataTypes::_ivec4:
			switch(element.layoutDataType)
			{
			case LayoutDataType::Float32:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case LayoutDataType::Uint32:
				format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case LayoutDataType::Uint16:
				format = DXGI_FORMAT_R16G16B16A16_UINT;
				break;
			case LayoutDataType::Uint8:
				format = DXGI_FORMAT_R8G8B8A8_UINT;
				break;
			case LayoutDataType::Int32:
				format = DXGI_FORMAT_R32G32B32A32_SINT;
				break;
			case LayoutDataType::Int16:
				format = DXGI_FORMAT_R16G16B16A16_SINT;
				break;
			case LayoutDataType::Int8:
				format = DXGI_FORMAT_R8G8B8A8_SINT;
				break;
			default:
				THROW("Unsupported layout data type for 4-vector data type");
			}
			break;
		case DataTypes::_mat4x4:
			THROW("Matrices can't be used in attributes");
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
