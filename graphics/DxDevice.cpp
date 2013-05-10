#include "DxDevice.hpp"
#include "DxSystem.hpp"
#include "DxContext.hpp"
#include "DxTexture.hpp"
#include "Win32Output.hpp"
#include "DxPresenter.hpp"
#include "DxRenderBuffer.hpp"
#include "DxDepthStencilBuffer.hpp"
#include "DxCompiledShader.hpp"
#include "DxVertexShader.hpp"
#include "DxPixelShader.hpp"
#include "DxUniformBuffer.hpp"
#include "DxVertexBuffer.hpp"
#include "DxIndexBuffer.hpp"
#include "VertexLayout.hpp"
#include "DxAttributeBinding.hpp"
#include "AttributeLayout.hpp"
#include "Image2DData.hpp"
#include "DxSamplerState.hpp"
#include "DxBlendState.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"
#include "d3dx.hpp"

BEGIN_INANITY_GRAPHICS

DxDevice::DxDevice(ptr<DxSystem> system, ComPointer<IDXGIAdapter> adapter, ComPointer<ID3D11Device> device, ptr<DxContext> context)
: system(system), adapter(adapter), device(device), context(context)
{
}

ptr<System> DxDevice::GetSystem() const
{
	return system;
}

DXGI_MODE_DESC DxDevice::GetClosestSupportedMode(const DXGI_MODE_DESC& mode) const
{
	try
	{
		// получить output по умолчанию
		IDXGIOutput* dxgiOutputInterface;
		if(FAILED(adapter->EnumOutputs(0, &dxgiOutputInterface)))
			THROW_PRIMARY_EXCEPTION("Can't get containing output for swap chain");

		ComPointer<IDXGIOutput> dxgiOutput = dxgiOutputInterface;

		DXGI_MODE_DESC closestMode;
		if(FAILED(dxgiOutput->FindClosestMatchingMode(&mode, &closestMode, device)))
			THROW_PRIMARY_EXCEPTION("Can't get closest matching mode");

		return closestMode;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get closest supported mode", exception);
	}
}

ptr<Presenter> DxDevice::CreatePresenter(ptr<Output> abstractOutput, const PresentMode& mode)
{
	try
	{
		// область вывода - только Win32
		ptr<Win32Output> output = abstractOutput.DynamicCast<Win32Output>();
		if(!output)
			THROW_PRIMARY_EXCEPTION("Only Win32 output allowed");

		// сформировать структуру настроек swap chain
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// подзадача - сформировать структуру режима экрана
		DXGI_MODE_DESC& modeDesc = desc.BufferDesc;
		modeDesc = DxSystem::GetModeDesc(mode);
		// если режим - полноэкранный, необходимо его скорректировать,
		// то есть привести к поддерживаемому
		if(mode.fullscreen)
			modeDesc = GetClosestSupportedMode(modeDesc);

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
			THROW_PRIMARY_EXCEPTION("Can't create swap chain");

		ComPointer<IDXGISwapChain> swapChain = swapChainInterface;

		// создать Presenter
		ptr<Presenter> presenter = NEW(DxPresenter(this, output, swapChain));

		// переключить режим
		presenter->SetMode(mode);

		// всё
		return presenter;
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

ptr<RenderBuffer> DxDevice::CreateRenderBuffer(int width, int height, PixelFormat pixelFormat)
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
			desc.Format = DxSystem::GetDXGIFormat(pixelFormat);
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			HRESULT hr;
			if(FAILED(hr = device->CreateTexture2D(&desc, 0, &bufferInterface)))
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
		}
		ComPointer<ID3D11Texture2D> buffer = bufferInterface;

		ID3D11RenderTargetView* renderTargetViewInterface;
		if(FAILED(device->CreateRenderTargetView(buffer, 0, &renderTargetViewInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create render target view");
		ComPointer<ID3D11RenderTargetView> renderTargetView = renderTargetViewInterface;

		ID3D11ShaderResourceView* shaderResourceViewInterface;
		if(FAILED(device->CreateShaderResourceView(buffer, 0, &shaderResourceViewInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create shader resource view");
		ComPointer<ID3D11ShaderResourceView> shaderResourceView = shaderResourceViewInterface;

		return NEW(DxRenderBuffer(renderTargetView, NEW(DxTexture(shaderResourceView))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create render buffer", exception);
	}
}

ptr<DepthStencilBuffer> DxDevice::CreateDepthStencilBuffer(int width, int height, bool canBeResource)
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
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
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
				THROW_PRIMARY_EXCEPTION("Can't create depth stencil view");
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
				THROW_PRIMARY_EXCEPTION("Can't create shader resource view");
			shaderResourceView = shaderResourceViewInterface;
			return NEW(DxDepthStencilBuffer(depthStencilView, NEW(DxTexture(shaderResourceView))));
		}

		return NEW(DxDepthStencilBuffer(depthStencilView));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create depth stencil buffer", exception);
	}
}

ptr<VertexShader> DxDevice::CreateVertexShader(ptr<File> file)
{
	try
	{
		ptr<DxCompiledShader> compiledShader = DxCompiledShader::Deserialize(NEW(FileInputStream(file)));
		ptr<File> code = compiledShader->GetCode();

		ID3D11VertexShader* shader;
		if(FAILED(device->CreateVertexShader(code->GetData(), code->GetSize(), NULL, &shader)))
			THROW_PRIMARY_EXCEPTION("Can't create DirectX 11 vertex shader");

		return NEW(DxVertexShader(code, shader, compiledShader->GetResources()));
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
		ptr<DxCompiledShader> compiledShader = DxCompiledShader::Deserialize(NEW(FileInputStream(file)));
		ptr<File> code = compiledShader->GetCode();

		ID3D11PixelShader* shader;
		if(FAILED(device->CreatePixelShader(code->GetData(), code->GetSize(), NULL, &shader)))
			THROW_PRIMARY_EXCEPTION("Can't create DirectX 11 pixel shader");

		return NEW(DxPixelShader(shader, compiledShader->GetResources()));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create pixel shader", exception);
	}
}

ptr<UniformBuffer> DxDevice::CreateUniformBuffer(int size)
{
	try
	{
		// округлить размер вверх, чтобы он был кратен sizeof(float4)
		// кроме того, если размер 0, то сделать не 0
		int ceiledSize = size;
		if(!ceiledSize) ceiledSize = 1;
		ceiledSize = (ceiledSize + sizeof(float4) - 1) & ~(sizeof(float4) - 1);

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = ceiledSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		ID3D11Buffer* buffer;
		if(FAILED(device->CreateBuffer(&desc, NULL, &buffer)))
			THROW_PRIMARY_EXCEPTION("Can't create buffer");
		return NEW(DxUniformBuffer(buffer, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create uniform buffer", exception);
	}
}

ptr<VertexBuffer> DxDevice::CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout)
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
			THROW_PRIMARY_EXCEPTION("Can't create vertex buffer");

		return NEW(DxVertexBuffer(vertexBufferInterface, file->GetSize() / layout->GetStride(), layout));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create vertex buffer", exception);
	}
}

ptr<IndexBuffer> DxDevice::CreateStaticIndexBuffer(ptr<File> file, int indexSize)
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
			THROW_PRIMARY_EXCEPTION("Can't create index buffer");

		return NEW(DxIndexBuffer(indexBufferInterface, file->GetSize() / indexSize, indexSize));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create index buffer", exception);
	}
}

ptr<AttributeBinding> DxDevice::CreateAttributeBinding(ptr<AttributeLayout> layout)
{
	try
	{
		ptr<DxAttributeBinding> binding = NEW(DxAttributeBinding());
		binding->Initialize(layout);
		return binding;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create DX attribute binding", exception);
	}
}

ptr<Texture> DxDevice::CreateStaticTexture(ptr<File> file)
{
	try
	{
		D3DX11_IMAGE_INFO info;
		if(FAILED(D3DX11GetImageInfoFromMemory(file->GetData(), file->GetSize(), 0, &info, NULL)))
			THROW_PRIMARY_EXCEPTION("Can't get image info");

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = info.Width;
		loadInfo.Height = info.Height;
		loadInfo.Depth = info.Depth;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = info.MipLevels;
		loadInfo.Usage = D3D11_USAGE_IMMUTABLE;
		loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags = 0;
		loadInfo.MiscFlags = info.MiscFlags;
		loadInfo.Format = info.Format;
		loadInfo.Filter = D3DX11_FILTER_LINEAR;
		loadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		loadInfo.pSrcInfo = &info;

		ID3D11ShaderResourceView* shaderResourceView;
		if(FAILED(D3DX11CreateShaderResourceViewFromMemory(device, file->GetData(), file->GetSize(), &loadInfo, 0, &shaderResourceView, NULL)))
			THROW_PRIMARY_EXCEPTION("Can't create shader resource view");

		return NEW(DxTexture(shaderResourceView));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create static texture", exception);
	}
}

ptr<Texture> DxDevice::CreateStatic2DTexture(ptr<Image2DData> imageData)
{
	try
	{
		// получить формат пикселов текстуры
		DXGI_FORMAT pixelFormat = DxSystem::GetDXGIFormat(imageData->GetPixelFormat());

		// создать текстуру
		ComPointer<ID3D11Texture2D> texture;
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = imageData->GetWidth();
			desc.Height = imageData->GetHeight();
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = pixelFormat;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA dataDesc;
			dataDesc.pSysMem = imageData->GetData()->GetData();
			dataDesc.SysMemPitch = imageData->GetPitch();

			ID3D11Texture2D* textureInterface;
			if(FAILED(device->CreateTexture2D(&desc, &dataDesc, &textureInterface)))
				THROW_PRIMARY_EXCEPTION("Can't create texture 2D");
			texture = textureInterface;
		}

		// создать shader resource
		ComPointer<ID3D11ShaderResourceView> shaderResourceView;
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = pixelFormat;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = -1;

			ID3D11ShaderResourceView* shaderResourceViewInterface;
			if(FAILED(device->CreateShaderResourceView(texture, &desc, &shaderResourceViewInterface)))
				THROW_PRIMARY_EXCEPTION("Can't create shader resource");
			shaderResourceView = shaderResourceViewInterface;
		}

		// вернуть текстуру
		return NEW(DxTexture(shaderResourceView));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create static 2D texture", exception);
	}
}

ptr<SamplerState> DxDevice::CreateSamplerState()
{
	return NEW(DxSamplerState(this));
}

ptr<BlendState> DxDevice::CreateBlendState()
{
	return NEW(DxBlendState(this));
}

ID3D11Device* DxDevice::GetDeviceInterface() const
{
	return device;
}

END_INANITY_GRAPHICS
