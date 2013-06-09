#include "Win32Monitor.hpp"
#include "../Strings.hpp"

BEGIN_INANITY_GRAPHICS

Win32Monitor::Win32Monitor(const DISPLAY_DEVICE& info)
: info(info), modesInitialized(false)
{
	deviceName = Strings::Unicode2UTF8(info.DeviceName);
	deviceString = Strings::Unicode2UTF8(info.DeviceString);
}

String Win32Monitor::GetId() const
{
	return deviceName;
}

String Win32Monitor::GetName() const
{
	return deviceString;
}

const std::vector<ptr<MonitorMode> >& Win32Monitor::GetModes()
{
	if(!modesInitialized)
	{
		for(int i = 0; ; ++i)
		{
			DEVMODE modeInfo;
			modeInfo.dmSize = sizeof(modeInfo);
			modeInfo.dmDriverExtra = 0;
			if(!EnumDisplaySettingsEx(info.DeviceName, i, &modeInfo, 0))
				break;
			modes.push_back(NEW(Win32MonitorMode(modeInfo)));
		}

		modesInitialized = true;
	}

	return modes;
}

ptr<MonitorMode> Win32Monitor::TryCreateMode(int width, int height)
{
	// TODO
	return 0;
}

END_INANITY_GRAPHICS
