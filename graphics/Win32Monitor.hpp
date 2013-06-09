#ifndef ___INANITY_GRAPHICS_WIN32_MONITOR_HPP___
#define ___INANITY_GRAPHICS_WIN32_MONITOR_HPP___

BEGIN_INANITY_GRAPHICS

/// Класс монитора Win32.
class Win32Monitor : public Monitor
{
private:
	DISPLAY_DEVICE info;
	String deviceName;
	String deviceString;

public:
	Win32Monitor(const DISPLAY_DEVICE& info);

	//*** методы Monitor
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<MonitorMode> >& GetModes();
	ptr<MonitorMode> TryCreateMode(int width, int height);
};

END_INANITY_GRAPHICS

#endif
