#ifndef ___INANITY_PLATFORM_SDL_HPP___
#define ___INANITY_PLATFORM_SDL_HPP___

#include "platform.hpp"
#include <SDL2/SDL.h>

BEGIN_INANITY

class Exception;

END_INANITY

BEGIN_INANITY_PLATFORM

class Sdl : public Object
{
private:
	static Sdl* instance;
	static Uint32 initializedSubsystems;

	Sdl();

public:
	~Sdl();

	static ptr<Sdl> Get(Uint32 flags = 0);
	static ptr<Exception> Error();
};

END_INANITY_PLATFORM

#endif
