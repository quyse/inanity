#include "Game.hpp"
#include "../Exception.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "../graphics/Dx11System.hpp"
#if defined(___INANITY_PLATFORM_XBOX)
#include "CoreWindow.hpp"
#include "../input/CoreManager.hpp"
#else
#include "Win32Window.hpp"
#include "../input/Win32RawManager.hpp"
#endif
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)
#include "Sdl.hpp"
#include "SdlWindow.hpp"
#include "../input/SdlManager.hpp"
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
#include "EmsWindow.hpp"
#include "Sdl.hpp"
#include "../input/SdlManager.hpp"
#else
#error Unknown platform
#endif

#include "../input/Frame.hpp"
#include "../graphics/GlSystem.hpp"
#if defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_MACOS)
#include "../audio/SdlSystem.hpp"
#elif defined(___INANITY_PLATFORM_SWITCH)
#include "../audio/AlSystem.hpp"
#endif

BEGIN_INANITY_PLATFORM

ptr<Graphics::System> Game::CreateDefaultGraphicsSystem()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
#if defined(___INANITY_PLATFORM_XBOX)
	return NEW(Graphics::Dx11System());
#else
	if(Graphics::Dx11System::IsSupported())
		return NEW(Graphics::Dx11System());
	else
		return NEW(Graphics::GlSystem());
#endif
#else
	return NEW(Graphics::GlSystem());
#endif
}

ptr<Audio::System> Game::CreateDefaultAudioSystem()
{
#if defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_MACOS)
	return NEW(Audio::SdlSystem());
#elif defined(___INANITY_PLATFORM_SWITCH)
	return NEW(Audio::AlSystem());
#else
	THROW("No audio system supported");
#endif
}

ptr<Input::Manager> Game::CreateInputManager(ptr<Window> window)
{
	BEGIN_TRY();

#if defined(___INANITY_PLATFORM_WINDOWS)
#if defined(___INANITY_PLATFORM_XBOX)
	ptr<Platform::CoreWindow> coreWindow = window.DynamicCast<Platform::CoreWindow>();
	ptr<Input::CoreManager> inputManager = NEW(Input::CoreManager(coreWindow));
	coreWindow->SetInputManager(inputManager);
	return inputManager;
#else
	ptr<Platform::Win32Window> win32Window = window.DynamicCast<Platform::Win32Window>();
	ptr<Input::Win32Manager> inputManager = NEW(Input::Win32RawManager(win32Window->GetHWND()));
	win32Window->SetInputManager(inputManager);
	return inputManager;
#endif
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)
	ptr<Platform::SdlWindow> sdlWindow = window.DynamicCast<Platform::SdlWindow>();
	ptr<Input::SdlManager> inputManager = NEW(Input::SdlManager());
	sdlWindow->SetInputManager(inputManager);
	return inputManager;
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	ptr<Platform::EmsWindow> emsWindow = window.DynamicCast<Platform::EmsWindow>();
	ptr<Input::SdlManager> inputManager = NEW(Input::SdlManager());
	emsWindow->SetInputManager(inputManager);
	return inputManager;
#else
#error Unknown platform
#endif

	END_TRY("Can't create input manager");
}

END_INANITY_PLATFORM
