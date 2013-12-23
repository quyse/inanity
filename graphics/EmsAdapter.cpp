#include "EmsAdapter.hpp"
#include "EmsMonitor.hpp"

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

void EmsAdapter::GetAdapters(Adapters& adapters)
{
	adapters.push_back(NEW(EmsAdapter()));
}

END_INANITY_GRAPHICS
