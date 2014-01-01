#include "SdlMonitor.hpp"
#include "SdlMonitorMode.hpp"
#include "../platform/Sdl.hpp"
#include "../platform/SdlWindow.hpp"
#include "../Exception.hpp"
#include <SDL2/SDL_video.h>

BEGIN_INANITY_GRAPHICS

SdlMonitor::SdlMonitor(int displayIndex) :
	sdl(Platform::Sdl::Get()),
	displayIndex(displayIndex),
	modesInitialized(false)
{
	BEGIN_TRY();

	const char* displayName = SDL_GetDisplayName(displayIndex);
	if(!displayName)
		THROW_SECONDARY("Can't get display name", Platform::Sdl::Error());
	name = displayName;

	END_TRY("Can't create SDL monitor");
}

String SdlMonitor::GetId() const
{
	return name;
}

String SdlMonitor::GetName() const
{
	return name;
}

const Monitor::MonitorModes& SdlMonitor::GetModes()
{
	BEGIN_TRY();

	if(!modesInitialized)
	{
		int modesCount = SDL_GetNumDisplayModes(displayIndex);
		if(modesCount <= 0)
			THROW_SECONDARY("Can't get modes count", Platform::Sdl::Error());

		modes.resize(modesCount);
		for(int i = 0; i < modesCount; ++i)
		{
			SDL_DisplayMode mode;
			if(SDL_GetDisplayMode(displayIndex, i, &mode) < 0)
				THROW_SECONDARY("Can't get display mode", Platform::Sdl::Error());

			modes[i] = NEW(SdlMonitorMode(mode));
		}

		modesInitialized = true;
	}

	return modes;

	END_TRY("Can't get SDL monitor modes");
}

ptr<MonitorMode> SdlMonitor::TryCreateMode(int width, int height)
{
	BEGIN_TRY();

	SDL_DisplayMode mode, closestMode;
	mode.format = SDL_PIXELFORMAT_RGBX8888;
	mode.w = width;
	mode.h = height;
	mode.refresh_rate = 0;
	mode.driverdata = 0;
	if(!SDL_GetClosestDisplayMode(displayIndex, &mode, &closestMode))
		THROW_SECONDARY("Can't get closest display mode", Platform::Sdl::Error());

	return NEW(SdlMonitorMode(closestMode));

	END_TRY("Can't try to create SDL mode");
}

ptr<Platform::Window> SdlMonitor::CreateDefaultWindow(const String& title, int width, int height)
{
	SDL_Window* handle = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED, // x
		SDL_WINDOWPOS_CENTERED, // y
		width,
		height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if(!handle)
		THROW_SECONDARY("Can't create SDL default window", Platform::Sdl::Error());

	return NEW(Platform::SdlWindow(handle));
}

END_INANITY_GRAPHICS
