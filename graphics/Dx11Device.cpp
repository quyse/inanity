#include "Dx11Device.hpp"
#include "Dx11System.hpp"
#include "Dx11Texture.hpp"
#include "Dx11SwapChainPresenter.hpp"
#include "Dx11ShaderCompiler.hpp"
#include "shaders/Hlsl11Generator.hpp"
#include "Dx11FrameBuffer.hpp"
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
#include "../platform/Win32Window.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

Dx11Device::Dx11Device(ptr<Dx11System> system, ComPointer<ID3D11Device> device, ComPointer<ID3D11DeviceContext> deviceContext)
: system(system), device(device), deviceContext(deviceContext)
{
	// setup caps
	caps.flags = Caps::attributeInstancing | Caps::drawInstancing;
	caps.maxColorBuffersCount = std::min(FrameBuffer::maxColorBuffersCount, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);
}

ptr<System> Dx11Device::GetSystem() const
{
	return system;
}

ptr<Presenter> Dx11Device::CreateWindowPresenter(ptr<Platform::Window> abstractWindow, ptr<MonitorMode> abstractMode)
{
	BEGIN_TRY();

	ptr<Platform::Win32Window> window = abstractWindow.DynamicCast<Platform::Win32Window>();
	if(!window)
		THROW("Only Win32 window is allowed");
	ptr<DxgiMonitorMode> mode = abstractMode.DynamicCast<DxgiMonitorMode>();
	if(!mode && abstractMode)
		THROW("Only DXGI monitor mode allowed");

	return CreatePresenter(window, mode);

	END_TRY("Can't create DirectX 11 window presenter");
}

ptr<Dx11SwapChainPresenter> Dx11Device::CreatePresenter(ptr<Platform::Win32Window> window, ptr<DxgiMonitorMode> mode)
{
	BEGIN_TRY();

	// сформировать структуру настроек swap chain
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BufferDesc = Dx11System::GetModeDesc(mode, window->GetClientWidth(), window->GetClientHeight());

	// мультисемплинга пока нет
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.OutputWindow = window->GetHWND();
	// согласно рекомендации SDK, даже в случае полного экрана, лучше
	// создавать в оконном режиме, а потом переключать
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// создать swap chain
	ComPointer<IDXGISwapChain> swapChain;
	if(FAILED(system->GetDXGIFactory()->CreateSwapChain(device, &desc, &swapChain)))
		THROW("Can't create swap chain");

	// создать Presenter
	ptr<Dx11SwapChainPresenter> presenter = NEW(Dx11SwapChainPresenter(this, window, swapChain));

	// переключить режим
	presenter->SetMode(mode);

	// всё
	return presenter;

	END_TRY("Can't create DirectX 11 swap chain presenter");
}

ptr<ShaderCompiler> Dx11Device::CreateShaderCompiler()
{
	return NEW(Dx11ShaderCompiler());
}

ptr<Shaders::ShaderGenerator> Dx11Device::CreateShaderGenerator()
{
	return NEW(Shaders::Hlsl11Generator());
}

ptr<FrameBuffer> Dx11Device::CreateFrameBuffer()
{
	return NEW(Dx11FrameBuffer(this));
}

ptr<Dx11RenderBuffer> Dx11Device::InternalCreateRenderBuffer(int width, int height, PixelFormat pixelFormat, const SamplerSettings& samplerSettings, bool gdiCompatible)
{
	BEGIN_TRY();

	ComPointer<ID3D11Texture2D> buffer;
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		// GDI compatibility requires B8G8R8A8 format
		desc.Format = gdiCompatible ? DXGI_FORMAT_B8G8R8A8_UNORM : Dx11System::GetDXGIFormat(pixelFormat);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = gdiCompatible ? D3D11_RESOURCE_MISC_GDI_COMPATIBLE : 0;
		HRESULT hr;
		if(FAILED(hr = device->CreateTexture2D(&desc, 0, &buffer)))
			THROW("Can't create buffer");
	}

	ComPointer<ID3D11RenderTargetView> renderTargetView;
	if(FAILED(device->CreateRenderTargetView(buffer, 0, &renderTargetView)))
		THROW("Can't create render target view");

	ComPointer<ID3D11ShaderResourceView> shaderResourceView;
	if(FAILED(device->CreateShaderResourceView(buffer, 0, &shaderResourceView)))
		THROW("Can't create shader resource view");

	return NEW(Dx11RenderBuffer(
		renderTargetView,
		NEW(Dx11Texture(
			shaderResourceView,
			CreateSamplerState(samplerSettings).FastCast<Dx11SamplerState>()))));

	END_TRY("Can't create DirectX 11 render buffer");
}

ptr<RenderBuffer> Dx11Device::CreateRenderBuffer(int width, int height, PixelFormat pixelFormat, const SamplerSettings& samplerSettings)
{
	return InternalCreateRenderBuffer(width, height, pixelFormat, samplerSettings, false);
}

ptr<Dx11RenderBuffer> Dx11Device::CreateRenderBufferGdiCompatible(int width, int height, const SamplerSettings& samplerSettings)
{
	return InternalCreateRenderBuffer(width, height, PixelFormats::uintRGBA32, samplerSettings, true);
}

ptr<DepthStencilBuffer> Dx11Device::CreateDepthStencilBuffer(int width, int height, bool canBeResource)
{
	try
	{
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
			if(FAILED(device->CreateTexture2D(&desc, 0, &buffer)))
				THROW("Can't create buffer");
		}

		ComPointer<ID3D11DepthStencilView> depthStencilView;
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = 0;
			desc.Texture2D.MipSlice = 0;
			if(FAILED(device->CreateDepthStencilView(buffer, &desc, &depthStencilView)))
				THROW("Can't create depth stencil view");
		}

		if(canBeResource)
		{
			ComPointer<ID3D11ShaderResourceView> shaderResourceView;
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = 1;
			if(FAILED(device->CreateShaderResourceView(buffer, &desc, &shaderResourceView)))
				THROW("Can't create shader resource view");
			return NEW(Dx11DepthStencilBuffer(depthStencilView, NEW(Dx11Texture(shaderResourceView, nullptr))));
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

		ComPointer<ID3D11Buffer> vertexBuffer;
		if(FAILED(device->CreateBuffer(&desc, &data, &vertexBuffer)))
			THROW("Can't create vertex buffer");

		return NEW(Dx11VertexBuffer(vertexBuffer, (int)(file->GetSize() / layout->GetStride()), layout));
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

		ComPointer<ID3D11Buffer> vertexBuffer;
		if(FAILED(device->CreateBuffer(&desc, NULL, &vertexBuffer)))
			THROW("Can't create vertex buffer");

		return NEW(Dx11VertexBuffer(vertexBuffer, size / layout->GetStride(), layout));
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

		ComPointer<ID3D11Buffer> indexBuffer;
		if(FAILED(device->CreateBuffer(&desc, &data, &indexBuffer)))
			THROW("Can't create index buffer");

		return NEW(Dx11IndexBuffer(indexBuffer, (int)(file->GetSize() / indexSize), indexSize));
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

ptr<Texture> Dx11Device::CreateStaticTexture(ptr<RawTextureData> data, const SamplerSettings& samplerSettings)
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

		ptr<Dx11SamplerState> samplerState = CreateSamplerState(samplerSettings).FastCast<Dx11SamplerState>();

		ComPointer<ID3D11ShaderResourceView> shaderResourceView;
		if(FAILED(device->CreateShaderResourceView(resource, &srvDesc, &shaderResourceView)))
			THROW("Can't create shader resource view");

		return NEW(Dx11Texture(shaderResourceView, samplerState));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create static DirectX 11 texture", exception);
	}
}

D3D11_TEXTURE_ADDRESS_MODE Dx11Device::ConvertSamplerSettingsWrap(SamplerSettings::Wrap wrap)
{
	switch(wrap)
	{
	case SamplerSettings::wrapRepeat:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	case SamplerSettings::wrapRepeatMirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case SamplerSettings::wrapClamp:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case SamplerSettings::wrapBorder:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}
	THROW("Invalid wrap mode");
}

ptr<SamplerState> Dx11Device::CreateSamplerState(const SamplerSettings& samplerSettings)
{
	BEGIN_TRY();

	D3D11_SAMPLER_DESC desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());

	// в DirectX общий параметр для фильтрации, так что разбираемся

	static bool haveFilterConversionMap = false;
	// параметры: min, mag, mip (как в DX, и не как в Sampler)
	static D3D11_FILTER filterConversionMap[2][2][2] =
	{
		{
			{
				D3D11_FILTER_MIN_MAG_MIP_POINT,
				D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR
			},
			{
				D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
				D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR
			}
		},
		{
			{
				D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
				D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR
			},
			{
				D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
				D3D11_FILTER_MIN_MAG_MIP_LINEAR
			}
		}
	};

	// фильтрация
	desc.Filter = filterConversionMap[samplerSettings.minFilter][samplerSettings.magFilter][samplerSettings.mipFilter];
	// режимы свёртки
	desc.AddressU = ConvertSamplerSettingsWrap(samplerSettings.wrapU);
	desc.AddressV = ConvertSamplerSettingsWrap(samplerSettings.wrapV);
	desc.AddressW = ConvertSamplerSettingsWrap(samplerSettings.wrapW);
	// минимальный и максимальный LOD
	desc.MinLOD = samplerSettings.minLOD;
	desc.MaxLOD = samplerSettings.maxLOD;
	// цвет границы
	for(int i = 0; i < 4; ++i)
		desc.BorderColor[i] = samplerSettings.borderColor(i);

	// создать sampler state
	ComPointer<ID3D11SamplerState> samplerStateInterface;
	if(FAILED(device->CreateSamplerState(&desc, &samplerStateInterface)))
		THROW("Can't create sampler state");

	return NEW(Dx11SamplerState(samplerStateInterface));

	END_TRY("Can't create DirectX 11 sampler state");
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
