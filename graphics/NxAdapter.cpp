#include "NxAdapter.hpp"
#include "NxMonitor.hpp"
#include "../platform/NxWindow.hpp"

BEGIN_INANITY_GRAPHICS

NxAdapter::NxAdapter() : monitorsInitialized(false)
{
}

String NxAdapter::GetId() const
{
	return "NxAdapter";
}

String NxAdapter::GetName() const
{
	return "NxAdapter";
}

const Adapter::Monitors& NxAdapter::GetMonitors()
{
	BEGIN_TRY();

	if(!monitorsInitialized)
	{
		// init vi
		nn::vi::Initialize();

		// at the moment initialize just one display
		nn::vi::DisplayInfo displayInfo;
		if(nn::vi::ListDisplays(&displayInfo, 1) != 1)
			THROW("Can't list displays");
		ptr<NxMonitor> monitor = NEW(NxMonitor(displayInfo));

		monitors.assign(1, monitor);

		monitorsInitialized = true;
	}

	return monitors;

	END_TRY("Can't get Nx monitors");
}

ptr<Platform::Window> NxAdapter::CreateOptimizedWindow(const String& title, int left, int top, int width, int height)
{
	return NEW(Platform::NxWindow());
}

void NxAdapter::GetAdapters(Adapters& adapters)
{
	int adaptersCount = 1;
	for(int i = 0; i < adaptersCount; ++i)
		adapters.push_back(NEW(NxAdapter()));
}

END_INANITY_GRAPHICS
