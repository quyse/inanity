#ifndef ___INANITY_GRAPHICS_X11_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_X11_ADAPTER_HPP___

#include "Adapter.hpp"
#include "../String.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class X11Display;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// X11 Display class.
class X11Adapter : public Adapter
{
private:
	String displayName;
	ptr<Platform::X11Display> display;
	std::vector<ptr<Monitor> > monitors;
	bool monitorsInitialized;

public:
	X11Adapter(const String& displayName, ptr<Platform::X11Display> display);

	/// Adapter's methods
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<Monitor> >& GetMonitors();

	ptr<Platform::X11Display> GetDisplay() const;

	static void GetAdapters(std::vector<ptr<Adapter> >& adapters);
};

END_INANITY_GRAPHICS

#endif
