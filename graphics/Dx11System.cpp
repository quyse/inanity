#include "Dx11System.hpp"
#include "Dx11Device.hpp"
#include "Dx11Context.hpp"
#include "Dx11ShaderCompiler.hpp"
#include "shaders/Hlsl11Generator.hpp"
#include "DxgiAdapter.hpp"
#include "../Win32Window.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

IDXGIFactory* Dx11System::GetDXGIFactory()
{
	if(!dxgiFactory)
	{
		IDXGIFactory* dxgiFactoryInterface;
		if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactoryInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create DXGI Factory");
		dxgiFactory = dxgiFactoryInterface;
	}
	return dxgiFactory;
}

DXGI_FORMAT Dx11System::GetDXGIFormat(PixelFormat format)
{
	switch(format)
	{
	case PixelFormats::unknown:
		return DXGI_FORMAT_UNKNOWN;
	case PixelFormats::intR8G8B8A8:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PixelFormats::floatR11G11B10:
		return DXGI_FORMAT_R11G11B10_FLOAT;
	case PixelFormats::floatR16:
		return DXGI_FORMAT_R16_FLOAT;
	case PixelFormats::typelessR32:
		return DXGI_FORMAT_R32_TYPELESS;
	case PixelFormats::floatR32:
		return DXGI_FORMAT_R32_FLOAT;
	case PixelFormats::floatR32Depth:
		return DXGI_FORMAT_D32_FLOAT;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

DXGI_MODE_DESC Dx11System::GetModeDesc(const PresentMode& mode)
{
	DXGI_MODE_DESC modeDesc;
	ZeroMemory(&modeDesc, sizeof(modeDesc));
	modeDesc.Width = (UINT)mode.width;
	modeDesc.Height = (UINT)mode.height;
	modeDesc.RefreshRate.Numerator = 0;
	modeDesc.RefreshRate.Denominator = 0;
	modeDesc.Format = GetDXGIFormat(mode.pixelFormat);
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	return modeDesc;
}

String Dx11System::GetSemanticString(int semantic)
{
	// имя семантики получается переводом кода семантики в 26-ричную систему (буквы)
	String name;
	name.assign(8, 'A');
	for(int j = 0; semantic; ++j)
	{
		name[j] = 'A' + semantic % 26;
		semantic /= 26;
	}
	return name;
}

Dx11System::Dx11System() : adaptersInitialized(false) {}

const std::vector<ptr<Adapter> >& Dx11System::GetAdapters()
{
	if(!adaptersInitialized)
		try
		{
			IDXGIFactory* factory = GetDXGIFactory();

			for(UINT i = 0; ; ++i)
			{
				IDXGIAdapter* adapterInterface;
				HRESULT hr = factory->EnumAdapters(i, &adapterInterface);
				if(SUCCEEDED(hr))
					adapters.push_back(NEW(DxgiAdapter(adapterInterface)));
				else if(hr == DXGI_ERROR_NOT_FOUND)
					break;
				else
					THROW_PRIMARY_EXCEPTION("Error enumerating adapters");
			}

			adaptersInitialized = true;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't get adapters of DirectX 11 system", exception);
		}

	return adapters;
}

ptr<Window> Dx11System::CreateDefaultWindow()
{
	return Win32Window::CreateForDirectX();
}

ptr<Device> Dx11System::CreatePrimaryDevice()
{
	try
	{
		// получить адаптер
		IDXGIAdapter* adapterInterface;
		if(FAILED(GetDXGIFactory()->EnumAdapters(0, &adapterInterface)))
			THROW_PRIMARY_EXCEPTION("Can't get primary adapter");
		ComPointer<IDXGIAdapter> adapter = adapterInterface;

		// флаги устройства
		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED
#ifdef _DEBUG
			| D3D11_CREATE_DEVICE_DEBUG
#endif
			;
		// уровень устройства - DirectX 10
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_10_0;
		// создать устройство
		ID3D11Device* deviceInterface;
		ID3D11DeviceContext* deviceContextInterface;
		D3D_FEATURE_LEVEL featureLevelSupported;
		// здесь необходимо указывать D3D_DRIVER_TYPE_UNKNOWN, если мы указываем adapter.
		// http://msdn.microsoft.com/en-us/library/ff476082 (remarks)
		if(FAILED(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, flags, &featureLevel, 1, D3D11_SDK_VERSION, &deviceInterface, &featureLevelSupported, &deviceContextInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create device and context");

		ComPointer<ID3D11Device> device = deviceInterface;
		ComPointer<ID3D11DeviceContext> deviceContext = deviceContextInterface;

		ptr<Dx11Context> context = NEW(Dx11Context(device, deviceContext));

		// вернуть объект
		return NEW(Dx11Device(this, adapter, device, context));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create primary device", exception);
	}
}

ptr<ShaderCompiler> Dx11System::CreateShaderCompiler()
{
	return NEW(Dx11ShaderCompiler());
}

ptr<Shaders::ShaderGenerator> Dx11System::CreateShaderGenerator()
{
	return NEW(Shaders::Hlsl11Generator());
}

END_INANITY_GRAPHICS
