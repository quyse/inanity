#ifndef ___INANITY_GRAPHICS_DXGI_MONITOR_MODE_HPP___
#define ___INANITY_GRAPHICS_DXGI_MONITOR_MODE_HPP___

#include "MonitorMode.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс режима монитора DXGI.
class DxgiMonitorMode : public MonitorMode
{
private:
	DXGI_MODE_DESC desc;
	String name;

public:
	DxgiMonitorMode(const DXGI_MODE_DESC& desc);

	//*** методы MonitorMode
	int GetWidth() const;
	int GetHeight() const;
	float GetRefreshRate() const;
	String GetName() const;

	const DXGI_MODE_DESC& GetDesc() const;
};

END_INANITY_GRAPHICS

#endif
