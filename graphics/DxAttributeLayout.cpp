#include "DxAttributeLayout.hpp"
#include "DxDevice.hpp"
#include "DxSystem.hpp"
#include "VertexLayout.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

DxAttributeLayout::DxAttributeLayout(ptr<DxDevice> device)
: device(device) {}

ID3D11InputLayout* DxAttributeLayout::GetInputLayoutInterface() const
{
	return inputLayout;
}

void DxAttributeLayout::Finalize()
{
	try
	{
		if(inputLayout)
			THROW_PRIMARY_EXCEPTION("Already finalized");

		// структуры описания
		std::vector<D3D11_INPUT_ELEMENT_DESC> descs(elements.size());
		// строки для имён семантик
		std::vector<String> semanticNames(elements.size());

		// заполнить их
		for(size_t i = 0; i < elements.size(); ++i)
		{
			const Element& element = elements[i];

			// выбрать формат элемента
			DXGI_FORMAT format;
			switch(element.dataType)
			{
			case DataTypes::Float:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			case DataTypes::Float2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case DataTypes::Float3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case DataTypes::Float4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case DataTypes::Float4x4:
				THROW_PRIMARY_EXCEPTION("Matrices can't be used in attributes");
			case DataTypes::UInt:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case DataTypes::UInt2:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case DataTypes::UInt3:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case DataTypes::UInt4:
				format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Unknown element type");
			}

			// получить информацию о слоте
			const Slot& slot = slots[element.slot];

			String& semanticName = semanticNames[i];
			semanticName = DxSystem::GetSemanticString(i);

			D3D11_INPUT_ELEMENT_DESC& desc = descs[i];
			desc.SemanticName = semanticName.c_str();
			desc.SemanticIndex = 0;
			desc.Format = format;
			desc.InputSlot = element.slot;
			desc.AlignedByteOffset = element.offset;
			desc.InputSlotClass = slot.divisor ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = slot.divisor;
		}

		// сформировать код шейдера-заглушки
		// Шейдер:
		// struct I{};float4 VS(I i):SV_Position{return float4(0,0,0,1);}
		// Компиляция:
		// fxc.exe" /EF /O3 /Tvs_4_0 /Ges /Qstrip_reflect /Qstrip_debug /Fodummy.fxo dummy.fx
		static const char dummyShaderBytecode[] =
			"\x44\x58\x42\x43\x3B\x6B\x2E\x80\xE8\xF7\x36\x1E\x20\x91\x7E\xD0"
			"\x0A\x97\x49\x6A\x01\x00\x00\x00\xB4\x00\x00\x00\x03\x00\x00\x00"
			"\x2C\x00\x00\x00\x3C\x00\x00\x00\x70\x00\x00\x00\x49\x53\x47\x4E"
			"\x08\x00\x00\x00\x00\x00\x00\x00\x08\x00\x00\x00\x4F\x53\x47\x4E"
			"\x2C\x00\x00\x00\x01\x00\x00\x00\x08\x00\x00\x00\x20\x00\x00\x00"
			"\x00\x00\x00\x00\x01\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00"
			"\x0F\x00\x00\x00\x53\x56\x5F\x50\x6F\x73\x69\x74\x69\x6F\x6E\x00"
			"\x53\x48\x44\x52\x3C\x00\x00\x00\x40\x00\x01\x00\x0F\x00\x00\x00"
			"\x67\x00\x00\x04\xF2\x20\x10\x00\x00\x00\x00\x00\x01\x00\x00\x00"
			"\x36\x00\x00\x08\xF2\x20\x10\x00\x00\x00\x00\x00\x02\x40\x00\x00"
			"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x3F"
			"\x3E\x00\x00\x01";

		// создать разметку
		ID3D11InputLayout* inputLayoutInterface;
		if(FAILED(device->GetDeviceInterface()->CreateInputLayout(
			&*descs.begin(), (UINT)descs.size(),
			dummyShaderBytecode, sizeof(dummyShaderBytecode) - 1, &inputLayoutInterface
		)))
			THROW_PRIMARY_EXCEPTION("Can't create input layout");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't finalize DX attribute layout", exception);
	}
}

END_INANITY_GRAPHICS
