#include "NxAdapter.hpp"
#include "../platform/Window.hpp"

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
		// TODO
		int monitorsCount = 1;
		monitors.resize(monitorsCount);

		monitorsInitialized = true;
	}

	return monitors;

	END_TRY("Can't get SDL monitors");

}

ptr<Platform::Window> NxAdapter::CreateOptimizedWindow(const String& title, int left, int top, int width, int height)
{
	// TODO
	return nullptr;
}

void NxAdapter::GetAdapters(Adapters& adapters)
{
	// TODO
	int adaptersCount = 1;
	for(int i = 0; i < adaptersCount; ++i)
		adapters.push_back(nullptr);
}

END_INANITY_GRAPHICS
