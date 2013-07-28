#include "Dx11Device.hpp"
#include "Dx11System.hpp"
#include "Dx11Texture.hpp"
#include "Win32Output.hpp"
#include "Dx11Presenter.hpp"
#include "Dx11RenderBuffer.hpp"
#include "Dx11DepthStencilBuffer.hpp"
#include "Dx11CompiledShader.hpp"
#include "Dx11VertexShader.hpp"
#include "Dx11PixelShader.hpp"
#include "Dx11UniformBuffer.hpp"
#include "Dx11VertexBuffer.hpp"
#include "Dx11IndexBuffer.hpp"
#include "VertexLayout.hpp"
#include "Dx11AttributeBinding.hpp"
#include "AttributeLayout.hpp"
#include "RawTextureData.hpp"
#include "Dx11SamplerState.hpp"
#include "Dx11BlendState.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

Dx11Device::Dx11Device(ptr<Dx11System> system, ComPointer<ID3D11Device> device, ComPointer<ID3D11DeviceContext> deviceContext)
: system(system), device(device), deviceContext(deviceContext)
{}

ptr<System> Dx11Device::GetSystem() const
{
	return system;
}

ptr<Presenter> Dx11Device::CreatePresenter(ptr<Output> abstractOutput, ptr<MonitorMode> abstractMode)
{
	try
	{
		// область вывода - только Win32
		ptr<Win32Output> output = abstractOutput.DynamicCast<Win32Output>();
		if(!output)
			THROW("Only Win32 output allowed");
		// режим, если есть - только DXGI
		ptr<DxgiMonitorMode> mode = abstractMode.DynamicCast<DxgiMonitorMode>();
		if(!mode && abstractMode)
			THROW("Only DXGI monitor mode allowed");

		// сформировать структуру настроек swap chain
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.BufferDesc = Dx11System::GetModeDesc(mode, output);

		// мультисемплинга пока нет
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.OutputWindow = output->GetHWND();
		// согласно рекомендации SDK, даже в случае полного экрана, лучше
		// создавать в оконном режиме, а потом переключать
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// создать swap chain
		IDXGISwapChain* swapChainInterface;
		if(FAILED(system->GetDXGIFactory()->CreateSwapChain(device, &desc, &swapChainInterface)))
			THROW("Can't create swap chain");

		ComPointer<IDXGISwapChain> swapChain = swapChainInterface;

		// создать Presenter
		ptr<Presenter> presenter = NEW(Dx11Presenter(this, output, swapChain));

		// переключить режим
		presenter->SetMode(mode);

		// всё
		return presenter;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create presenter for DX device", exception);
	}
}

ptr<RenderBuffer> Dx11Device::CreateRenderBuffer(int width, int height, PixelFormat pixelFormat)
{
	try
	{
		ID3D11Texture2D* bufferInterface;
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = Dx11System::GetDXGIFormat(pixelFormat);
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			HRESULT hr;
			if(FAILED(hr = device->CreateTexture2D(&desc, 0, &bufferInterface)))
				THROW("Can't create buffer");
		}
		ComPointer<ID3D11Texture2D> buffer = bufferInterface;

		ID3D11RenderTargetView* renderTargetViewInterface;
		if(FAILED(device->CreateRenderTargetView(buffer, 0, &renderTargetViewInterface)))
			THROW("Can't create render target view");
		ComPointer<ID3D11RenderTargetView> renderTargetView = renderTargetViewInterface;

		ID3D11ShaderResourceView* shaderResourceViewInterface;
		if(FAILED(device->CreateShaderResourceView(buffer, 0, &shaderResourceViewInterface)))
			THROW("Can't create shader resource view");
		ComPointer<ID3D11ShaderResourceView> shaderResourceView = shaderResourceViewInterface;

		return NEW(Dx11RenderBuffer(renderTargetView, NEW(Dx11Texture(shaderResourceView))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create render buffer", exception);
	}
}

ptr<DepthStencilBuffer> Dx11Device::CreateDepthStencilBuffer(int width, int height, bool canBeResource)
{
	try
	{
		ID3D11Texture2D* bufferInterface;
		ComPointer<ID3D11Texture2D> buffer;
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = (canBeResource ? D3D11_BIND_SHADER_RESOURCE : 0) | D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			if(FAILED(device->CreateTexture2D(&desc, 0, &bufferInterface)))
				THROW("Can't create buffer");
			buffer = bufferInterface;
		}

		ID3D11DepthStencilView* depthStencilViewInterface;
		ComPointer<ID3D11DepthStencilView> depthStencilView;
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = 0;
			desc.Texture2D.MipSlice = 0;
			if(FAILED(device->CreateDepthStencilView(buffer, &desc, &depthStencilViewInterface)))
				THROW("Can't create depth stencil view");
			depthStencilView = depthStencilViewInterface;
		}

		if(canBeResource)
		{
			ID3D11ShaderResourceView* shaderResourceViewInterface;
			ComPointer<ID3D11ShaderResourceView> shaderResourceView;
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = 1;
			if(FAILED(device->CreateShaderResourceView(buffer, &desc, &shaderResourceViewInterface)))
				THROW("Can't create shader resource view");
			shaderResourceView = shaderResourceViewInterface;
			return NEW(Dx11DepthStencilBuffer(depthStencilView, NEW(Dx11Texture(shaderResourceView))));
		}

		return NEW(Dx11DepthStencilBuffer(depthStencilView));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create depth stencil buffer", exception);
	}
}

ptr<VertexShader> Dx11Device::CreateVertexShader(ptr<File> file)
{
	try
	{
		ptr<Dx11CompiledShader> compiledShader = Dx11CompiledShader::Deserialize(NEW(FileInputStream(file)));
		ptr<File> code = compiledShader->GetCode();

		ID3D11VertexShader* shader;
		if(FAILED(device->CreateVertexShader(code->GetData(), code->GetSize(), NULL, &shader)))
			THROW("Can't create DirectX 11 vertex shader");

		return NEW(Dx11VertexShader(code, shader, compiledShader->GetResources()));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create vertex shader", exception);
	}
}

ptr<PixelShader> Dx11Device::CreatePixelShader(ptr<File> file)
{
	try
	{
		ptr<Dx11CompiledShader> compiledShader = Dx11CompiledShader::Deserialize(NEW(FileInputStream(file)));
		ptr<File> code = compiledShader->GetCode();

		ID3D11PixelShader* shader;
		if(FAILED(device->CreatePixelShader(code->GetData(), code->GetSize(), NULL, &shader)))
			THROW("Can't create DirectX 11 pixel shader");

		return NEW(Dx11PixelShader(shader, compiledShader->GetResources()));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create pixel shader", exception);
	}
}

ptr<UniformBuffer> Dx11Device::CreateUniformBuffer(int size)
{
	try
	{
		// округлить размер вверх, чтобы он был кратен sizeof(vec4)
		// кроме того, если размер 0, то сделать не 0
		int ceiledSize = size;
		if(!ceiledSize) ceiledSize = 1;
		ceiledSize = (ceiledSize + sizeof(vec4) - 1) & ~(sizeof(vec4) - 1);

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = ceiledSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		ID3D11Buffer* buffer;
		if(FAILED(device->CreateBuffer(&desc, NULL, &buffer)))
			THROW("Can't create buffer");
		return NEW(Dx11UniformBuffer(buffer, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create uniform buffer", exception);
	}
}

ptr<VertexBuffer> Dx11Device::CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout)
{
	try
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = (UINT)file->GetSize();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = file->GetData();

		ID3D11Buffer* vertexBufferInterface;
		if(FAILED(device->CreateBuffer(&desc, &data, &vertexBufferInterface)))
			THROW("Can't create vertex buffer");

		return NEW(Dx11VertexBuffer(vertexBufferInterface, file->GetSize() / layout->GetStride(), layout));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create DirectX 11 static vertex buffer", exception);
	}
}

ptr<VertexBuffer> Dx11Device::CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout)
{
	try
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = (UINT)size;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		ID3D11Buffer* vertexBufferInterface;
		if(FAILED(device->CreateBuffer(&desc, NULL, &vertexBufferInterface)))
			THROW("Can't create vertex buffer");

		return NEW(Dx11VertexBuffer(vertexBufferInterface, size / layout->GetStride(), layout));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create DirectX 11 dynamic vertex buffer", exception);
	}
}

ptr<IndexBuffer> Dx11Device::CreateStaticIndexBuffer(ptr<File> file, int indexSize)
{
	try
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = (UINT)file->GetSize();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = file->GetData();

		ID3D11Buffer* indexBufferInterface;
		if(FAILED(device->CreateBuffer(&desc, &data, &indexBufferInterface)))
			THROW("Can't create index buffer");

		return NEW(Dx11IndexBuffer(indexBufferInterface, file->GetSize() / indexSize, indexSize));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create index buffer", exception);
	}
}

ptr<AttributeBinding> Dx11Device::CreateAttributeBinding(ptr<AttributeLayout> layout)
{
	try
	{
		ptr<Dx11AttributeBinding> binding = NEW(Dx11AttributeBinding());
		binding->Initialize(layout);
		return binding;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create DX attribute binding", exception);
	}
}

ptr<Texture> Dx11Device::CreateStaticTexture(ptr<RawTextureData> data)
{
	try
	{
		int mips = data->GetImageMips();
		int realCount = data->GetCount();
		if(realCount == 0)
			realCount = 1;
		DXGI_FORMAT format = Dx11System::GetDXGIFormat(data->GetFormat());

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ComPointer<ID3D11Resource> resource;

		// if it is a 3D texture
		if(data->GetImageDepth())
		{
			// 3D arrays are not supported
			if(data->GetCount())
				THROW("Arrays of 3D textures are not supported");

			{
				// create texture and upload data
				D3D11_TEXTURE3D_DESC desc;
				desc.Width = data->GetImageWidth();
				desc.Height = data->GetImageHeight();
				desc.Depth = data->GetImageDepth();
				desc.MipLevels = mips;
				desc.Format = format;
				desc.Usage = D3D11_USAGE_IMMUTABLE;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;

				std::vector<D3D11_SUBRESOURCE_DATA> dataDescs(mips);
				for(int mip = 0; mip < mips; ++mip)
				{
					D3D11_SUBRESOURCE_DATA& dataDesc = dataDescs[mip];
					dataDesc.pSysMem = data->GetMipData(0, mip);
					dataDesc.SysMemPitch = data->GetMipLinePitch(mip);
					dataDesc.SysMemSlicePitch = data->GetMipSlicePitch(mip);
				}

				ID3D11Texture3D* textureInterface;
				if(FAILED(device->CreateTexture3D(&desc, &*dataDescs.begin(), &textureInterface)))
					THROW("Can't create texture 3D");
				resource = textureInterface;
			}

			// fill SRV desc
			srvDesc.Format = format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			srvDesc.Texture3D.MostDetailedMip = 0;
			srvDesc.Texture3D.MipLevels = -1;
		}
		// if it is a 2D texture
		else if(data->GetImageHeight())
		{
			{
				// create texture and upload data
				D3D11_TEXTURE2D_DESC desc;
				desc.Width = data->GetImageWidth();
				desc.Height = data->GetImageHeight();
				desc.MipLevels = mips;
				desc.ArraySize = realCount;
				desc.Format = format;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_IMMUTABLE;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;

				std::vector<D3D11_SUBRESOURCE_DATA> dataDescs(realCount * mips);
				for(int image = 0; image < realCount; ++image)
					for(int mip = 0; mip < mips; ++mip)
					{
						D3D11_SUBRESOURCE_DATA& dataDesc = dataDescs[image * mips + mip];
						dataDesc.pSysMem = data->GetMipData(image, mip);
						dataDesc.SysMemPitch = data->GetMipLinePitch(mip);
						dataDesc.SysMemSlicePitch = 0;
					}

				ID3D11Texture2D* textureInterface;
				if(FAILED(device->CreateTexture2D(&desc, &*dataDescs.begin(), &textureInterface)))
					THROW("Can't create texture 2D");
				resource = textureInterface;
			}

			// fill SRV desc
			srvDesc.Format = format;
			if(data->GetCount())
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.MostDetailedMip = 0;
				srvDesc.Texture2DArray.MipLevels = -1;
				srvDesc.Texture2DArray.FirstArraySlice = 0;
				srvDesc.Texture2DArray.ArraySize = realCount;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = -1;
			}
		}
		// else it is 1D texture
		else
		{
			{
				// create texture and upload data
				D3D11_TEXTURE1D_DESC desc;
				desc.Width = data->GetImageWidth();
				desc.MipLevels = mips;
				desc.ArraySize = realCount;
				desc.Format = format;
				desc.Usage = D3D11_USAGE_IMMUTABLE;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;

				std::vector<D3D11_SUBRESOURCE_DATA> dataDescs(realCount * mips);
				for(int image = 0; image < realCount; ++image)
					for(int mip = 0; mip < mips; ++mip)
					{
						D3D11_SUBRESOURCE_DATA& dataDesc = dataDescs[image * mips + mip];
						dataDesc.pSysMem = data->GetMipData(image, mip);
						dataDesc.SysMemPitch = 0;
						dataDesc.SysMemSlicePitch = 0;
					}

				ID3D11Texture1D* textureInterface;
				if(FAILED(device->CreateTexture1D(&desc, &*dataDescs.begin(), &textureInterface)))
					THROW("Can't create texture 1D");
				resource = textureInterface;
			}

			// fill SRV desc
			srvDesc.Format = format;
			if(data->GetCount())
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture1DArray.MostDetailedMip = 0;
				srvDesc.Texture1DArray.MipLevels = -1;
				srvDesc.Texture1DArray.FirstArraySlice = 0;
				srvDesc.Texture1DArray.ArraySize = realCount;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture1D.MostDetailedMip = 0;
				srvDesc.Texture1D.MipLevels = -1;
			}
		}

		ID3D11ShaderResourceView* shaderResourceViewInterface;
		if(FAILED(device->CreateShaderResourceView(resource, &srvDesc, &shaderResourceViewInterface)))
			THROW("Can't create shader resource view");

		return NEW(Dx11Texture(shaderResourceViewInterface));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create static DirectX 11 texture", exception);
	}
}

ptr<SamplerState> Dx11Device::CreateSamplerState()
{
	return NEW(Dx11SamplerState());
}

ptr<BlendState> Dx11Device::CreateBlendState()
{
	return NEW(Dx11BlendState());
}

ID3D11Device* Dx11Device::GetDeviceInterface() const
{
	return device;
}

ID3D11DeviceContext* Dx11Device::GetDeviceContextInterface() const
{
	return deviceContext;
}

END_INANITY_GRAPHICS
