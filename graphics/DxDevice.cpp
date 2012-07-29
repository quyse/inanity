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
		return NEW(DxVertexShader(shader));
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
