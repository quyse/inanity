#include "DxSystem.hpp"
#include "DxDevice.hpp"
#include "DxContext.hpp"
#include "../Win32Window.hpp"
#include "../Exception.hpp"

IDXGIFactory* DxSystem::GetDXGIFactory()
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

DXGI_FORMAT DxSystem::GetDXGIFormat(PixelFormat format)
{
	switch(format)
	{
	case PixelFormats::unknown:
		return DXGI_FORMAT_UNKNOWN;
	case PixelFormats::intR8G8B8A8:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PixelFormats::floatR11G11B10:
		return DXGI_FORMAT_R11G11B10_FLOAT;
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

ptr<Window> DxSystem::CreateDefaultWindow()
{
	return Win32Window::CreateForDirectX();
}

ptr<Device> DxSystem::CreatePrimaryDevice()
{
	try
	{
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
		if(FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, &featureLevel, 1, D3D11_SDK_VERSION, &deviceInterface, &featureLevelSupported, &deviceContextInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create device and context");

		ComPointer<ID3D11Device> device = deviceInterface;
		ptr<DxContext> context = NEW(DxContext(deviceContextInterface));

		// вернуть объект
		return NEW(DxDevice(this, deviceInterface, context));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create primary device", exception);
	}
}
