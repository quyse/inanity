#include "Sdl.hpp"
#include <SDL.h>

BEGIN_INANITY_PLATFORM

Sdl* Sdl::instance = nullptr;

Sdl::Sdl()
{
	instance = this;
	SDL_Init(0);
}

Sdl::~Sdl()
{
	instance = nullptr;
	SDL_Quit();
}

ptr<Sdl> Sdl::Get()
{
	if(!instance)
		instance = NEW(Sdl());
	return instance;
}

END_INANITY_PLATFORM
