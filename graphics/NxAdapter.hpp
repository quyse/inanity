#ifndef ___INANITY_GRAPHICS_NX_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_NX_ADAPTER_HPP___

#include "Adapter.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_GRAPHICS

/// Nintendo adapter.
class NxAdapter : public Adapter
{
private:
	Monitors monitors;
	bool monitorsInitialized;

public:
	NxAdapter();

	//*** Adapter's methods.
	String GetId() const;
	String GetName() const;
	const Monitors& GetMonitors();
	ptr<Platform::Window> CreateOptimizedWindow(const String& title, int left, int top, int width, int height);

	static void GetAdapters(Adapters& adapters);
};

END_INANITY_GRAPHICS

#endif
