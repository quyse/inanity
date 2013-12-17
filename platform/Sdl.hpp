#ifndef ___INANITY_PLATFORM_SDL_HPP___
#define ___INANITY_PLATFORM_SDL_HPP___

#include "platform.hpp"

BEGIN_INANITY_PLATFORM

class Sdl : public Object
{
private:
	static Sdl* instance;

	Sdl();

public:
	~Sdl();

	static ptr<Sdl> Get();
};

END_INANITY_PLATFORM

#endif
