#include "GlSystem.hpp"
#ifdef ___INANITY_WINDOWS
#include "../windows.hpp"
#endif

ptr<Window> GlSystem::CreateWindow()
{
#ifdef ___INANITY_WINDOWS
	return Win32Window::CreateForOpenGL();
#endif
#ifdef ___INANITY_LINUX
	return NEW(X11Window());
#endif
}

ptr<Device> GlSystem::CreatePrimaryDevice()
{
	try
	{
#ifdef ___INANITY_WINDOWS
		// получить всё устройства
		DISPLAY_DEVICE deviceInfo;
		deviceInfo.cb = sizeof(deviceInfo);
		for(DWORD i = 0; EnumDisplayDevices(NULL, i, &deviceInfo, 0); ++i)
			// если устройство primary
			if(deviceInfo.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				// вернуть его
				return NEW(GlDevice(this, deviceInfo.DeviceName, 0));

		// в MSDN написано, устройство всегда есть, так что этого быть не должно
		THROW_PRIMARY_EXCEPTION("Can't find primary device");
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create primary device", exception);
	}
}
