#include "DxgiAdapter.hpp"
#include "DxgiMonitor.hpp"
#include "../platform/Win32Window.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <sstream>

BEGIN_INANITY_GRAPHICS

DxgiAdapter::DxgiAdapter(ComPointer<IDXGIAdapter> adapter)
: adapter(adapter), monitorsInitialized(false)
{
	try
	{
		if(FAILED(adapter->GetDesc(&desc)))
			THROW("Can't get adapter desc");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create DXGI Adapter", exception);
	}
}

String DxgiAdapter::GetId() const
{
	std::stringstream ss;
	ss << desc.AdapterLuid.HighPart << '.' << desc.AdapterLuid.LowPart;
	return ss.str();
}

String DxgiAdapter::GetName() const
{
	return Strings::Unicode2UTF8(desc.Description);
}

const Adapter::Monitors& DxgiAdapter::GetMonitors()
{
	if(!monitorsInitialized)
		try
		{
			for(UINT i = 0; ; ++i)
			{
				ComPointer<IDXGIOutput> outputInterface;
				HRESULT hr = adapter->EnumOutputs(i, &outputInterface);
				if(SUCCEEDED(hr))
					monitors.push_back(NEW(DxgiMonitor(outputInterface)));
				else if(hr == DXGI_ERROR_NOT_FOUND)
					break;
				else
					THROW("Error enumerating outputs");
			}

			monitorsInitialized = true;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY("Can't get monitors of DXGI adapter", exception);
		}

	return monitors;
}

ptr<Platform::Window> DxgiAdapter::CreateOptimizedWindow(const String& title, int left, int top, int width, int height)
{
	return Platform::Win32Window::CreateForDirectX(title, left, top, width, height);
}

IDXGIAdapter* DxgiAdapter::GetInterface() const
{
	return adapter;
}

END_INANITY_GRAPHICS
