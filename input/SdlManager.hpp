#ifndef ___INANITY_INPUT_SDL_MANAGER_HPP___
#define ___INANITY_INPUT_SDL_MANAGER_HPP___

#include "Manager.hpp"
#include "Controller.hpp"
#include "../platform/platform.hpp"
#include <unordered_map>

#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
// workaround for emscripten
#include <SDL/SDL_events.h>
#else
#include <SDL2/SDL_events.h>
#endif

BEGIN_INANITY_PLATFORM

class Sdl;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

class SdlManager : public Manager
{
private:
	ptr<Platform::Sdl> sdl;
	float widthScale;
	float heightScale;

	class SdlController : public Controller
	{
	private:
		mutable CriticalSection criticalSection;
		SDL_GameController* controller;

	public:
		SdlController(SDL_GameController* controller);
		~SdlController();

		bool IsActive() const;

		void Close();
	};

	std::unordered_map<SDL_JoystickID, ptr<SdlController> > controllers;

public:
	SdlManager();

	void ProcessEvent(const SDL_Event& event);
	void SetVirtualScale(float widthScale, float heightScale);
	ptr<Controller> TryGetController(int controllerId);
};

END_INANITY_INPUT

#endif
