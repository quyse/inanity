#ifndef ___INANITY_INPUT_SDL_MANAGER_HPP___
#define ___INANITY_INPUT_SDL_MANAGER_HPP___

#include "Manager.hpp"
#include "Controller.hpp"
#include "../platform/platform.hpp"
#include <unordered_map>

// emscripten's SDL does not support controllers
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
#include <SDL/SDL_events.h>
#else
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_haptic.h>
#endif

BEGIN_INANITY_PLATFORM

class Sdl;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

class SdlManager : public Manager
{
private:
	ptr<Platform::Sdl> sdl;
	float widthScale, heightScale;
	int lastCursorX, lastCursorY;

#if !defined(___INANITY_PLATFORM_EMSCRIPTEN)
	class SdlController : public Controller
	{
	private:
		mutable CriticalSection criticalSection;
		SDL_GameController* controller;
		SDL_Haptic* haptic;
		SDL_HapticEffect hapticEffect;
		int hapticEffectIndex;

		SdlController(SDL_GameController* controller, SDL_Joystick* joystick);

	public:
		SdlController(SDL_GameController* controller);
		~SdlController();

		bool IsActive() const;
		void RunHapticLeftRight(float left, float right);
		void StopHaptic();

		void Close();
	};

	std::unordered_map<uint64_t, ptr<SdlController> > controllers;
#endif

public:
	SdlManager();

	void ProcessEvent(const SDL_Event& event);
	void SetVirtualScale(float widthScale, float heightScale);
	void StartTextInput();
	void StopTextInput();
#if !defined(___INANITY_PLATFORM_EMSCRIPTEN)
	ptr<Controller> TryGetController(uint64_t controllerId);

	void TryLoadControllerMappingsFromFile(const char* fileName);
#endif
};

END_INANITY_INPUT

#endif
