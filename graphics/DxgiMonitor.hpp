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
	std::vector<ptr<MonitorMode> > modes;
	bool modesInitialized;

public:
	DxgiMonitor(ComPointer<IDXGIOutput> output);

	//*** методы Monitor
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<MonitorMode> >& GetModes();
};

END_INANITY_GRAPHICS

#endif
