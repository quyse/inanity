#include "Desktop.hpp"
#include "../graphics/Monitor.hpp"
#include "../Exception.hpp"
#ifdef ___INANITY_WINDOWS
#include "../graphics/DxgiMonitor.hpp"
#include "Win32Window.hpp"
#include "../windows.hpp"
#endif

BEGIN_INANITY_PLATFORM

ptr<Window> Desktop::CreateWindowCentered(const String& title, ptr<Graphics::Monitor> abstractMonitor, int width, int height)
{
	try
	{
#ifdef ___INANITY_WINDOWS
		{
			ptr<Graphics::DxgiMonitor> monitor = abstractMonitor.DynamicCast<Graphics::DxgiMonitor>();
			const DXGI_OUTPUT_DESC& desc = monitor->GetDesc();
			MONITORINFO info;
			info.cbSize = sizeof(info);
			if(!GetMonitorInfo(desc.Monitor, &info))
				THROW_PRIMARY_EXCEPTION("Can't get monitor info");
			const RECT& rect = info.rcMonitor;
			return Win32Window::CreateForDirectX(
				title,
				rect.left + (rect.right - rect.left - width) / 2,
				rect.top + (rect.bottom - rect.top - height) / 2,
				width, height
				);
		}
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create window", exception);
	}
}

END_INANITY_PLATFORM
