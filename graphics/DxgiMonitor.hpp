#ifndef ___INANITY_GRAPHICS_DXGI_MONITOR_HPP___
#define ___INANITY_GRAPHICS_DXGI_MONITOR_HPP___

#include "Monitor.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс устройства вывода DXGI.
class DxgiMonitor : public Monitor
{
private:
	ComPointer<IDXGIOutput> output;
	DXGI_OUTPUT_DESC desc;
	MonitorModes modes;
	bool modesInitialized;

public:
	DxgiMonitor(ComPointer<IDXGIOutput> output);

	//*** методы Monitor
	String GetId() const;
	String GetName() const;
	const MonitorModes& GetModes();
	ptr<MonitorMode> TryCreateMode(int width, int height);
	RECT GetRect() const;
	ptr<Platform::Window> CreateDefaultWindow(const String& title, int width, int height);

	const DXGI_OUTPUT_DESC& GetDesc() const;
};

END_INANITY_GRAPHICS

#endif
