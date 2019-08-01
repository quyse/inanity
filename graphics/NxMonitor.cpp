#include "NxMonitor.hpp"
#include "MonitorMode.hpp"
#include "../platform/NxWindow.hpp"

BEGIN_INANITY_GRAPHICS

const Monitor::MonitorModes NxMonitor::modes;

NxMonitor::NxMonitor(const nn::vi::DisplayInfo& displayInfo)
: displayInfo(displayInfo) {}

NxMonitor::~NxMonitor()
{
}

String NxMonitor::GetId() const
{
	return displayInfo.name;
}

String NxMonitor::GetName() const
{
	return displayInfo.name;
}

const Monitor::MonitorModes& NxMonitor::GetModes()
{
	return modes;
}

ptr<MonitorMode> NxMonitor::TryCreateMode(int width, int height)
{
	return nullptr;
}

ptr<Platform::Window> NxMonitor::CreateDefaultWindow(const String& title, int width, int height)
{
	return NEW(Platform::NxWindow());
}

END_INANITY_GRAPHICS
