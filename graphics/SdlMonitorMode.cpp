#include "SdlMonitorMode.hpp"
#include "../platform/Sdl.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

SdlMonitorMode::SdlMonitorMode(const SDL_DisplayMode& mode)
: mode(mode)
{
	BEGIN_TRY();

	// create name
	// TODO: add pixel format
	std::ostringstream ss;
	ss << "SDL " << mode.w << 'x' << mode.h << ", " << mode.refresh_rate << " Hz";
	name = ss.str();

	END_TRY("Can't create SDL monitor mode");
}

int SdlMonitorMode::GetWidth() const
{
	return mode.w;
}

int SdlMonitorMode::GetHeight() const
{
	return mode.h;
}

float SdlMonitorMode::GetRefreshRate() const
{
	return (float)mode.refresh_rate;
}

String SdlMonitorMode::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
