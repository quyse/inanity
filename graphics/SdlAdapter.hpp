#ifndef ___INANITY_GRAPHICS_SDL_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_SDL_ADAPTER_HPP___

#include "Adapter.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class Sdl;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// SDL adapter.
class SdlAdapter : public Adapter
{
private:
	ptr<Platform::Sdl> sdl;
	String name;
	Monitors monitors;
	bool monitorsInitialized;

public:
	SdlAdapter(int videoDriverIndex);

	//*** Adapter's methods.
	String GetId() const;
	String GetName() const;
	const Monitors& GetMonitors();

	static void GetAdapters(Adapters& adapters);
};

END_INANITY_GRAPHICS

#endif
