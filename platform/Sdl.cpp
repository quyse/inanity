#include "Sdl.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

Sdl* Sdl::instance = nullptr;
Uint32 Sdl::initializedSubsystems = 0;

Sdl::Sdl()
{
	instance = this;
	initializedSubsystems = 0;
	SDL_Init(0);
}

Sdl::~Sdl()
{
	instance = nullptr;
	if(initializedSubsystems)
	{
		SDL_QuitSubSystem(initializedSubsystems);
		initializedSubsystems = 0;
	}
	SDL_Quit();
}

ptr<Sdl> Sdl::Get(Uint32 flags)
{
	if(!instance)
		instance = NEW(Sdl());

	flags &= ~initializedSubsystems;
	if(flags)
	{
		if(SDL_InitSubSystem(flags) != 0)
			THROW_SECONDARY("Can't initialize SDL subsystem(s)", Error());
		initializedSubsystems |= flags;
	}

	return instance;
}

ptr<Exception> Sdl::Error()
{
	const char* errorString = SDL_GetError();
	if(!errorString || !*errorString)
		return nullptr;
	return NEW(Exception(errorString));
}

END_INANITY_PLATFORM
