#include "Win32Adapter.hpp"
#include "Win32Monitor.hpp"
#include "../Strings.hpp"

BEGIN_INANITY_GRAPHICS

Win32Adapter::Win32Adapter(const DISPLAY_DEVICE& info)
: info(info), monitorsInitialized(false)
{
	deviceName = Strings::Unicode2UTF8(info.DeviceName);
	deviceString = Strings::Unicode2UTF8(info.DeviceString);
}

String Win32Adapter::GetId() const
{
	return deviceName;
}

String Win32Adapter::GetName() const
{
	return deviceString;
}

const Adapter::Monitors& Win32Adapter::GetMonitors()
{
	if(!monitorsInitialized)
	{
		for(int i = 0; ; ++i)
		{
			DISPLAY_DEVICE monitorInfo;
			monitorInfo.cb = sizeof(monitorInfo);
			if(!EnumDisplayDevices(info.DeviceName, i, &monitorInfo, EDD_GET_DEVICE_INTERFACE_NAME))
				break;
			monitors.push_back(NEW(Win32Monitor(monitorInfo)));
		}

		monitorsInitialized = false;
	}

	return monitors;
}

void Win32Adapter::GetAdapters(Adapters& adapters)
{
	for(int i = 0; ; ++i)
	{
		DISPLAY_DEVICE adapterInfo;
		adapterInfo.cb = sizeof(adapterInfo);
		if(!EnumDisplayDevices(NULL, i, &adapterInfo, 0))
			break;
		adapters.push_back(NEW(Win32Adapter(adapterInfo)));
	}
}

END_INANITY_GRAPHICS
