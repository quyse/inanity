#ifndef ___INANITY_GRAPHICS_EMS_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_EMS_ADAPTER_HPP___

#include "Adapter.hpp"

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
#error EmsAdapter is for Emscripten platform
#endif

BEGIN_INANITY_GRAPHICS

/// Dummy adapter class for Emscripten graphics.
class EmsAdapter : public Adapter
{
private:
	std::vector<ptr<Monitor> > monitors;

public:
	EmsAdapter();

	//*** Adapter's methods.
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<Monitor> >& GetMonitors();

	static void GetAdapters(std::vector<ptr<Adapter> >& adapters);
};

END_INANITY_GRAPHICS

#endif
