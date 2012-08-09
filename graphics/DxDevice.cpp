#include "DxDevice.hpp"
#include "DxSystem.hpp"
#include "DxContext.hpp"
#include "DxTexture.hpp"
#include "Win32Output.hpp"
#include "DxPresenter.hpp"
#include "DxRenderBuffer.hpp"
#include "DxVertexShader.hpp"
#include "DxPixelShader.hpp"
#include "DxUniformBuffer.hpp"
#include "VertexLayout.hpp"
#include "DxInternalInputLayout.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

DxDevice::DxDevice(ptr<DxSystem> system, ComPointer<ID3D11Device> device, ptr<DxContext> context)
: system(system), device(device), context(context)
{
}

ptr<System> DxDevice::GetSystem() const
{
	return system;
}

ptr<Presenter> DxDevice::CreatePresenter(ptr<Output> abstractOutput, const PresentMode& mode)
{
	try
	{
		// область вывода - только Win32
		ptr<Win32Output> output = abstractOutput.DynamicCast<Win32Output>();
		if(!output)
			THROW_PRIMARY_EXCEPTION("Only Win32 output allowed");

		// создать Presenter
		return NEW(DxPresenter(this, output, mode));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create presenter for DX device", exception);
	}
}

ptr<Context> DxDevice::GetContext()
{
	return context;
}

ptr<RenderBuffer> DxDevice::CreateRenderBuffer(size_t width, size_t height, PixelFormat pixelFormat)
{
	try
	{
		ID3D11Texture2D* buffer;
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DxSystem::GetDXGIFormat(pixelFormat);
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			HRESULT hr;
			if(FAILED(hr = device->CreateTexture2D(&desc, 0, &buffer)))
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
		}

		ID3D11RenderTargetView* renderTargetView;
		if(FAILED(device->CreateRenderTargetView(buffer, 0, &renderTargetView)))
		{
			buffer->Release();
			THROW_PRIMARY_EXCEPTION("Can't create render target view");
		}

		ID3D11ShaderResourceView* shaderResourceView;
		if(FAILED(device->CreateShaderResourceView(buffer, 0, &shaderResourceView)))
		{
			buffer->Release();
			renderTargetView->Release();
			THROW_PRIMARY_EXCEPTION("Can't create shader resource view");
		}

		buffer->Release();

		return NEW(DxRenderBuffer(renderTargetView, NEW(DxTexture(shaderResourceView))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create render buffer", exception);
	}
}

ptr<VertexShader> DxDevice::CreateVertexShader(ptr<File> file)
{
	try
	{
		ID3D11VertexShader* shader;
		if(FAILED(device->CreateVertexShader(file->GetData(), file->GetSize(), NULL, &shader)))
			THROW_PRIMARY_EXCEPTION("Can't create DirectX 11 vertex shader");
		return NEW(DxVertexShader(file, shader));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create vertex shader", exception);
	}
}

ptr<PixelShader> DxDevice::CreatePixelShader(ptr<File> file)
{
	try
	{
		ID3D11PixelShader* shader;
		if(FAILED(device->CreatePixelShader(file->GetData(), file->GetSize(), NULL, &shader)))
			THROW_PRIMARY_EXCEPTION("Can't create DirectX 11 pixel shader");
		return NEW(DxPixelShader(shader));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create pixel shader", exception);
	}
}

ptr<UniformBuffer> DxDevice::CreateUniformBuffer(size_t size)
{
	try
	{
		D3D11_BUFFER_DESC desc;
		//округлить размер вверх, чтобы он был кратен sizeof(float4)
		size_t ceiledSize = (size + sizeof(float4) - 1) & ~(sizeof(float4) - 1);
		desc.ByteWidth = ceiledSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		ID3D11Buffer* buffer;
		HRESULT hr;
		if(FAILED(hr = device->CreateBuffer(&desc, NULL, &buffer)))
			THROW_PRIMARY_EXCEPTION("Can't create buffer");
		return NEW(DxUniformBuffer(buffer, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create shader constant buffer", exception);
	}
}

ID3D11Device* DxDevice::GetDeviceInterface() const
{
	return device;
}

ptr<DxInternalInputLayout> DxDevice::CreateInternalInputLayout(ptr<VertexLayout> vertexLayout, ptr<DxVertexShader> vertexShader)
{
	try
	{
		// получить элементы разметки
		const std::vector<VertexLayout::Element>& elements = vertexLayout->GetElements();
		if(elements.empty())
			THROW_PRIMARY_EXCEPTION("Vertex layout is empty");

		// структуры описания
		// количество не указывается, так как матрицы требуют несколько элементов
		std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
		// строки для имён семантик, их количество известно
		std::vector<String> semanticNames(elements.size());

		// и заполнить их
		for(size_t i = 0; i < elements.size(); ++i)
		{
			const VertexLayout::Element& element = elements[i];

			// получить количество необходимых описаний для элемента
			int needDescsCount;
			switch(element.type)
			{
			case VertexLayout::typeFloat4x4:
				needDescsCount = 4;
				break;
			default:
				needDescsCount = 1;
				break;
			}

			// выбрать формат элемента
			DXGI_FORMAT format;
			switch(element.type)
			{
			case VertexLayout::typeFloat:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			case VertexLayout::typeFloat2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case VertexLayout::typeFloat3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case VertexLayout::typeFloat4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case VertexLayout::typeFloat4x4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case VertexLayout::typeUInt:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case VertexLayout::typeUInt2:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case VertexLayout::typeUInt3:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case VertexLayout::typeUInt4:
				format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Unknown element type");
			}

			// имя семантики получается переводом кода семантики в 26-ричную систему (буквы)
			String& semanticName = semanticNames[i];
			semanticName.assign('A', 8);
			int semantic = element.semantic;
			for(int j = 0; semantic; ++j)
			{
				semanticName[j] = 'A' + semantic % 26;
				semantic /= 26;
			}

			// цикл по вот этим требуемым элементам
			for(int descIndex = 0; descIndex < needDescsCount; ++descIndex)
			{
				descs.push_back(D3D11_INPUT_ELEMENT_DESC());
				D3D11_INPUT_ELEMENT_DESC& desc = descs.back();

				// SemanticName в структуре будет заполнено позже,
				// потому что сейчас адреса в векторе ещё могут поменяться

				desc.SemanticName = semanticName.c_str();
				desc.SemanticIndex = descIndex;
				desc.Format = format;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = element.offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
			}
		}

		// получить код шейдера
		ptr<File> vertexShaderCode = vertexShader->GetCode();

		// создать разметку
		ID3D11InputLayout* inputLayoutInterface;
		if(FAILED(device->CreateInputLayout(&*descs.begin(), (UINT)descs.size(), vertexShaderCode->GetData(), vertexShaderCode->GetSize(), &inputLayoutInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create input layout");

		return NEW(DxInternalInputLayout(inputLayoutInterface));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create DX internal input layout", exception);
	}
}
