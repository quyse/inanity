#include "EmsMonitor.hpp"
#include "MonitorMode.hpp"
#include "../platform/EmsWindow.hpp"

BEGIN_INANITY_GRAPHICS

EmsMonitor::EmsMonitor() {}

String EmsMonitor::GetId() const
{
	return "EmsMonitor";
}

String EmsMonitor::GetName() const
{
	return GetId();
}

const Monitor::MonitorModes& EmsMonitor::GetModes()
{
	return modes;
}

ptr<MonitorMode> EmsMonitor::TryCreateMode(int width, int height)
{
	return 0;
}

ptr<Platform::Window> EmsMonitor::CreateDefaultWindow(const String& title, int width, int height)
{
	return NEW(Platform::EmsWindow(title, width, height));
}

END_INANITY_GRAPHICS
