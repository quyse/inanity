#include "EmsAdapter.hpp"
#include "EmsMonitor.hpp"
#include "../platform/EmsWindow.hpp"

BEGIN_INANITY_GRAPHICS

EmsAdapter::EmsAdapter() : monitors(1, NEW(EmsMonitor())) {}

String EmsAdapter::GetId() const
{
	return "EmsAdapter";
}

String EmsAdapter::GetName() const
{
	return GetId();
}

const Adapter::Monitors& EmsAdapter::GetMonitors()
{
	return monitors;
}

ptr<Platform::Window> EmsAdapter::CreateOptimizedWindow(const String& title, int left, int top, int width, int height)
{
	return NEW(Platform::EmsWindow(title, width, height));
}

void EmsAdapter::GetAdapters(Adapters& adapters)
{
	adapters.push_back(NEW(EmsAdapter()));
}

END_INANITY_GRAPHICS
