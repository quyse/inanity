#include "System.hpp"
#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "Texture.hpp"
#include "DepthStencilBuffer.hpp"
#include "ConstantBuffer.hpp"
#include "ShaderReflection.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "../Window.hpp"
#include "../../File.hpp"
#include "../../Exception.hpp"
#include <vector>

String System::staticResourceName = "/inanity/dx/System";

System::System(ptr<Graphics::Window> window, const SystemSettings& settings)
{
	try
	{
		//запомнить размеры и формат экрана
		screenSettings = settings.screenSettings;

		// сформировать структуру режима экрана
		DXGI_MODE_DESC mode = GetScreenMode(screenSettings);

		//если режим - полноэкранный, необходимо его скорректировать,
		//то есть привести к поддерживаемому
		if(settings.screenSettings.fullscreen)
			mode = GetSupportedScreenMode(mode);

		//создать цепочку буферов вывода
		{
			DXGI_SWAP_CHAIN_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BufferCount = 1;
			desc.BufferDesc = mode;
			desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = window->GetWindowHandle();
			//мультисемплинга пока нет
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			//согласно рекомендации SDK, даже в случае полного экрана, лучше
			//создавать в оконном режиме, а потом переключать
			desc.Windowed = TRUE;
			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_10_0;

			ID3D11Device* deviceInterface;
			IDXGISwapChain* swapChainInterface;
			ID3D11DeviceContext* deviceContextInterface;

			UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED
#ifdef _DEBUG
				| D3D11_CREATE_DEVICE_DEBUG
#endif
				;
			if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, settings.hardware ? D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_REFERENCE, NULL,
				flags, &featureLevel, 1, D3D11_SDK_VERSION, &desc, &swapChainInterface, &deviceInterface, NULL, &deviceContextInterface)))
				THROW_PRIMARY_EXCEPTION("Can't create device and swap chain");

			device = deviceInterface;
			swapChain = swapChainInterface;
			context = NEW(Context(deviceContextInterface));
		}

		/* Есть некая странная процедура отсюда:
		http://msdn.microsoft.com/en-us/library/ee417025(v=VS.85).aspx#Full_Screen_Issues,
		но она не работает в полноэкранном режиме. Поэтому сделано так.
		*/
		//запомнить режим, который мы хотим установить
		ScreenSettings needScreenSettings = screenSettings;
		//а текущий режим исправим, чтобы он соответствовал действительному положению дел
		screenSettings.width = 1;
		screenSettings.height = 1;
		screenSettings.fullscreen = true;
		//переключить режим
		SetScreenSettings(needScreenSettings);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't initialize graphics", exception);
	}
}

DXGI_MODE_DESC System::GetScreenMode(const ScreenSettings& screenSettings)
{
	DXGI_MODE_DESC mode;
	mode.Width = screenSettings.width;
	mode.Height = screenSettings.height;
	mode.Format = screenFormat;
	mode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	mode.RefreshRate.Numerator = 0;
	mode.RefreshRate.Denominator = 0;
	mode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	return mode;
}

DXGI_MODE_DESC System::GetSupportedScreenMode(const DXGI_MODE_DESC& modeDesc)
{
	try
	{
		//создать фабрику DXGI
		IDXGIFactory* factoryPtr;
		if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factoryPtr)))
			THROW_PRIMARY_EXCEPTION("Can't create DXGI Factory");
		ComPointer<IDXGIFactory> factory = factoryPtr;

		//получить первый попавшийся адаптер
		IDXGIAdapter* adapterPtr;
		if(FAILED(factory->EnumAdapters(0, &adapterPtr)))
			THROW_PRIMARY_EXCEPTION("Can't get default adapter");
		ComPointer<IDXGIAdapter> adapter = adapterPtr;

		//получить первое попавшееся устройство вывода
		IDXGIOutput* outputPtr;
		if(FAILED(adapter->EnumOutputs(0, &outputPtr)))
			THROW_PRIMARY_EXCEPTION("Can't get default adapter's output");
		ComPointer<IDXGIOutput> output = outputPtr;

		//найти подходящий графический режим среди поддерживаемых устройством вывода
		DXGI_MODE_DESC bestModeDesc;
		if(FAILED(output->FindClosestMatchingMode(&modeDesc, &bestModeDesc, nullptr)))
			THROW_PRIMARY_EXCEPTION("Can't find closest matching mode");

		return bestModeDesc;	
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get supported screen mode", exception);
	}
}

const ScreenSettings& System::GetScreenSettings() const
{
	return screenSettings;
}

void System::SetScreenSettings(const ScreenSettings& screenSettings)
{
	try
	{
		//В MSDN рекомендуется сначала изменять размер, а потом переключать полноэкранность.

		//если режим изменился, изменить его
		if(this->screenSettings.width != screenSettings.width || this->screenSettings.height != screenSettings.height)
			if(FAILED(swapChain->ResizeTarget(&GetScreenMode(screenSettings))))
				THROW_PRIMARY_EXCEPTION("Can't resize target");
		//если полноэкранность изменилась, изменить её
		if(this->screenSettings.fullscreen != screenSettings.fullscreen)
			if(FAILED(swapChain->SetFullscreenState(screenSettings.fullscreen, nullptr)))
				THROW_PRIMARY_EXCEPTION("Can't set fullscreen state");

		//запомнить новые настройки
		this->screenSettings = screenSettings;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't set screen settings", exception);
	}
}

void System::Resize(size_t width, size_t height)
{
	//сбросить ссылку на вторичный буфер
	backBuffer = 0;
	//изменить размеры буфера
	if(FAILED(swapChain->ResizeBuffers(1, width, height, screenFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		THROW_PRIMARY_EXCEPTION("Can't resize buffers");
}

ptr<RenderBuffer> System::GetBackBuffer()
{
	if(!backBuffer)
	{
		ID3D11Texture2D* backBufferTexture;
		if(FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)))
			THROW_PRIMARY_EXCEPTION("Can't get back buffer texture");

		ID3D11RenderTargetView* renderTarget;
		HRESULT hr = device->CreateRenderTargetView(backBufferTexture, 0, &renderTarget);
		backBufferTexture->Release();
		if(FAILED(hr))
			THROW_PRIMARY_EXCEPTION("Can't create back buffer render target view");
		backBuffer = NEW(RenderBuffer(renderTarget));
	}
	return backBuffer;
}

void System::Flip()
{
	swapChain->Present(0, 0);
}

ptr<Context> System::GetContext() const
{
	return context;
}

ptr<RenderBuffer> System::CreateRenderBuffer(size_t width, size_t height, PixelFormat format)
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
			desc.Format = (DXGI_FORMAT)format;
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

		return NEW(RenderBuffer(renderTargetView, NEW(Texture(shaderResourceView))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create render buffer", exception);
	}
}

ptr<DepthStencilBuffer> System::CreateDepthStencilBuffer(size_t width, size_t height, PixelFormat format, bool canBeResource, PixelFormat depthStencilViewFormat, PixelFormat shaderResourceViewFormat)
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
			desc.Format = (DXGI_FORMAT)format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = (canBeResource ? D3D11_BIND_SHADER_RESOURCE : 0) | D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			if(FAILED(device->CreateTexture2D(&desc, 0, &buffer)))
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
		}

		ID3D11DepthStencilView* depthStencilView;
		{
			HRESULT hr;
			if(depthStencilViewFormat == pixelFormatUnknown)
				hr = device->CreateDepthStencilView(buffer, 0, &depthStencilView);
			else
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				desc.Format = (DXGI_FORMAT)depthStencilViewFormat;
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				desc.Flags = 0;
				desc.Texture2D.MipSlice = 0;
				hr = device->CreateDepthStencilView(buffer, &desc, &depthStencilView);
			}
			if(FAILED(hr))
			{
				buffer->Release();
				THROW_PRIMARY_EXCEPTION("Can't create depth stencil view");
			}
		}

		if(canBeResource)
		{
			ID3D11ShaderResourceView* shaderResourceView;
			HRESULT hr;
			if(shaderResourceViewFormat == pixelFormatUnknown)
				hr = device->CreateShaderResourceView(buffer, 0, &shaderResourceView);
			else
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				desc.Format = (DXGI_FORMAT)shaderResourceViewFormat;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MostDetailedMip = 0;
				desc.Texture2D.MipLevels = 1;
				hr = device->CreateShaderResourceView(buffer, &desc, &shaderResourceView);
			}
			if(FAILED(hr))
			{
				buffer->Release();
				depthStencilView->Release();
				THROW_PRIMARY_EXCEPTION("Can't create shader resource view");
			}
			buffer->Release();
			return NEW(DepthStencilBuffer(depthStencilView, NEW(Texture(shaderResourceView))));
		}

		buffer->Release();
		return NEW(DepthStencilBuffer(depthStencilView));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create depth stencil buffer", exception);
	}
}

ptr<ConstantBuffer> System::CreateConstantBuffer(size_t size)
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
		return NEW(ConstantBuffer(buffer, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create shader constant buffer", exception);
	}
}

ptr<VertexShader> System::CreateVertexShader(ptr<File> file)
{
	try
	{
		ID3D11VertexShader* shader;
		if(FAILED(device->CreateVertexShader(file->GetData(), file->GetSize(), NULL, &shader)))
			THROW_PRIMARY_EXCEPTION("Can't create DirectX 11 vertex shader");
		return NEW(VertexShader(NEW(ShaderReflection(file)), shader));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create vertex shader", exception);
	}
}

ptr<PixelShader> System::CreatePixelShader(ptr<File> file)
{
	try
	{
		ID3D11PixelShader* shader;
		if(FAILED(device->CreatePixelShader(file->GetData(), file->GetSize(), NULL, &shader)))
			THROW_PRIMARY_EXCEPTION("Can't create DirectX 11 pixel shader");
		return NEW(PixelShader(NEW(ShaderReflection(file)), shader));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create pixel shader", exception);
	}
}

/*
//ptr<Geometry> System::CreateStaticGeometry(ptr<File> vertices, unsigned vertexStride, ptr<File> indices, unsigned indexStride,

ptr<TextureBuffer> System::CreateTextureBuffer(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeReadByCPU)
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
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = canBeReadByCPU ? D3D11_USAGE_STAGING : D3D11_USAGE_DEFAULT;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = canBeReadByCPU ? D3D11_CPU_ACCESS_READ : 0;
			desc.MiscFlags = 0;
			HRESULT hr;
			if(FAILED(hr = device->CreateTexture2D(&desc, 0, &buffer)))
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
		}

		return NEW(TextureBuffer(buffer));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create texture buffer", exception);
	}
}

std::vector<ptr<RenderBuffer> > System::CreateRenderBuffersMipmap(unsigned width, unsigned height, unsigned levelsCount, DXGI_FORMAT format, bool canBeResource)
{
	try
	{
		//создать текстуру
		ID3D11Texture2D* buffer;
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = levelsCount;
			desc.ArraySize = 1;
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = (canBeResource ? D3D11_BIND_SHADER_RESOURCE : 0) | D3D11_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			HRESULT hr;
			if(FAILED(hr = device->CreateTexture2D(&desc, 0, &buffer)))
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
		}

		//создать рендертаргеты
		std::vector<ID3D11RenderTargetView*> renderTargetViews(levelsCount);
		for(int i = 0; i < (int)levelsCount; ++i)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = format;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = i;
			if(FAILED(device->CreateRenderTargetView(buffer, &desc, &renderTargetViews[i])))
			{
				buffer->Release();
				for(--i; i>=0; --i)
					renderTargetViews[i]->Release();
				THROW_PRIMARY_EXCEPTION("Can't create render target view");
			}
		}

		if(canBeResource)
		{
			std::vector<ID3D11ShaderResourceView*> shaderResourceViews(levelsCount);
			for(int i = 0; i < (int)levelsCount; ++i)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				desc.Format = format;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MostDetailedMip = i;
				desc.Texture2D.MipLevels = 1;
				if(FAILED(device->CreateShaderResourceView(buffer, &desc, &shaderResourceViews[i])))
				{
					buffer->Release();
					for(--i; i >= 0; --i)
						shaderResourceViews[i]->Release();
					for(i = 0; i < (int)levelsCount; ++i)
						renderTargetViews[i]->Release();
					THROW_PRIMARY_EXCEPTION("Can't create shader resource view");
				}
			}
			buffer->Release();
			std::vector<ptr<RenderBuffer>> renderBuffers(levelsCount);
			for(unsigned i = 0; i < levelsCount; ++i)
				renderBuffers[i] = NEW(RenderBuffer(renderTargetViews[i], shaderResourceViews[i]));
			return renderBuffers;
		}

		buffer->Release();
		std::vector<ptr<RenderBuffer>> renderBuffers(levelsCount);
		for(unsigned i = 0; i < levelsCount; ++i)
			renderBuffers[i] = NEW(RenderBuffer(renderTargetViews[i]));
		return renderBuffers;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create render buffers with mipmap chain", exception);
	}
}

std::pair<ptr<DepthStencilBuffer>, ptr<DepthStencilBuffer>> System::Create2DepthStencilBuffers(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeResource, DXGI_FORMAT depthStencilViewFormat1, DXGI_FORMAT depthStencilViewFormat2, DXGI_FORMAT shaderResourceViewFormat)
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
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = (canBeResource ? D3D11_BIND_SHADER_RESOURCE : 0) | D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			if(FAILED(device->CreateTexture2D(&desc, 0, &buffer)))
				THROW_PRIMARY_EXCEPTION("Can't create buffer");
		}

		ID3D11DepthStencilView* depthStencilView1;
		{
			HRESULT hr;
			if(depthStencilViewFormat1 == DXGI_FORMAT_UNKNOWN)
				hr = device->CreateDepthStencilView(buffer, 0, &depthStencilView1);
			else
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				desc.Format = depthStencilViewFormat1;
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				desc.Flags = 0;
				desc.Texture2D.MipSlice = 0;
				hr = device->CreateDepthStencilView(buffer, &desc, &depthStencilView1);
			}
			if(FAILED(hr))
			{
				buffer->Release();
				THROW_PRIMARY_EXCEPTION("Can't create depth stencil view 1");
			}
		}

		ID3D11DepthStencilView* depthStencilView2;
		{
			HRESULT hr;
			if(depthStencilViewFormat2 == DXGI_FORMAT_UNKNOWN)
				hr = device->CreateDepthStencilView(buffer, 0, &depthStencilView2);
			else
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				desc.Format = depthStencilViewFormat2;
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				desc.Flags = 0;
				desc.Texture2D.MipSlice = 0;
				hr = device->CreateDepthStencilView(buffer, &desc, &depthStencilView2);
			}
			if(FAILED(hr))
			{
				buffer->Release();
				depthStencilView1->Release();
				THROW_PRIMARY_EXCEPTION("Can't create depth stencil view 2");
			}
		}

		if(canBeResource)
		{
			ID3D11ShaderResourceView* shaderResourceView;
			HRESULT hr;
			if(shaderResourceViewFormat == DXGI_FORMAT_UNKNOWN)
				hr = device->CreateShaderResourceView(buffer, 0, &shaderResourceView);
			else
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				desc.Format = shaderResourceViewFormat;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MostDetailedMip = 0;
				desc.Texture2D.MipLevels = 1;
				hr = device->CreateShaderResourceView(buffer, &desc, &shaderResourceView);
			}
			if(FAILED(hr))
			{
				buffer->Release();
				depthStencilView1->Release();
				depthStencilView2->Release();
				THROW_PRIMARY_EXCEPTION("Can't create shader resource view");
			}
			buffer->Release();

			//добавить ссылку на resource view, так как он будет запихнут в два объекта
			shaderResourceView->AddRef();

			return std::make_pair(NEW(DepthStencilBuffer(depthStencilView1, shaderResourceView)), NEW(DepthStencilBuffer(depthStencilView2, shaderResourceView)));
		}

		buffer->Release();
		return std::make_pair(NEW(DepthStencilBuffer(depthStencilView1)), NEW(DepthStencilBuffer(depthStencilView2)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create depth stencil buffers", exception);
	}
}

ptr<OcclusionPredicate> System::CreateOcclusionPredicate()
{
	try
	{
		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_OCCLUSION_PREDICATE;
		desc.MiscFlags = 0;
		ID3D11Query* query;
		if(FAILED(device->CreateQuery(&desc, &query)))
			THROW_PRIMARY_EXCEPTION("Can't create query");
		return NEW(OcclusionPredicate(query));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create occlusion predicate", exception);
	}
}
*/

ID3D11Device* System::GetDevice() const
{
	return device;
}
