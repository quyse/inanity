#include "Win32Monitor.hpp"
#include "Win32MonitorMode.hpp"
#include "../platform/Win32Window.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"

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

ptr<Platform::Window> Win32Monitor::CreateWindowCentered(const String& title, int width, int height)
{
	try
	{
		RECT rect = GetRect();
		return Platform::Win32Window::CreateForOpenGL(
			title,
			rect.left + (rect.right - rect.left - width) / 2,
			rect.top + (rect.bottom - rect.top - height) / 2,
			width, height
			);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create window centered in Win32 monitor", exception);
	}
}

RECT Win32Monitor::GetRect() const
{
	try
	{
		DEVMODE modeInfo;
		modeInfo.dmSize = sizeof(modeInfo);
		modeInfo.dmDriverExtra = 0;
		std::cout << deviceName << "\n";
		if(!EnumDisplaySettingsEx(info.DeviceName, ENUM_CURRENT_SETTINGS, &modeInfo, 0))
			THROW_PRIMARY_EXCEPTION("Can't get current mode");

		RECT rect;
		rect.left = (modeInfo.dmFields & DM_POSITION) ? modeInfo.dmPosition.x : 0;
		rect.top = (modeInfo.dmFields & DM_POSITION) ? modeInfo.dmPosition.y : 0;
		rect.right = rect.left + ((modeInfo.dmFields & DM_PELSWIDTH) ? modeInfo.dmPelsWidth : 0);
		rect.bottom = rect.top + ((modeInfo.dmFields & DM_PELSHEIGHT) ? modeInfo.dmPelsHeight : 0);
		return rect;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get Win32 monitor rect", exception);
	}
}

END_INANITY_GRAPHICS
