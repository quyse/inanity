#ifndef ___INANITY_INPUT_SDL_MANAGER_HPP___
#define ___INANITY_INPUT_SDL_MANAGER_HPP___

#include "Manager.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class Sdl;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

class SdlManager : public Manager
{
private:
	ptr<Platform::Sdl> sdl;

public:
	SdlManager();
	~SdlManager();

	void Update();
};

END_INANITY_INPUT

#endif
