#ifndef ___INANITY_GRAPHICS_WIN32_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_WIN32_ADAPTER_HPP___

#include "Adapter.hpp"
#include "../platform/windows.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс графического адаптера Win32.
class Win32Adapter : public Adapter
{
private:
	String deviceName;
	String deviceString;
	DISPLAY_DEVICE info;
	Monitors monitors;
	bool monitorsInitialized;

public:
	Win32Adapter(const DISPLAY_DEVICE& info);

	//*** методы Adapter
	String GetId() const;
	String GetName() const;
	const Monitors& GetMonitors();

	static void GetAdapters(Adapters& adapters);
};

END_INANITY_GRAPHICS

#endif
