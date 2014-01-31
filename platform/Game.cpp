#include "Game.hpp"
#include "../Exception.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "Win32Window.hpp"
#include "../graphics/Dx11System.hpp"
#include "../input/Win32RawManager.hpp"
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
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

BEGIN_INANITY_PLATFORM

ptr<Graphics::System> Game::CreateDefaultGraphicsSystem()
{
#ifdef ___INANITY_PLATFORM_WINDOWS
	if(Graphics::Dx11System::IsSupported())
		return NEW(Graphics::Dx11System());
#endif
	return NEW(Graphics::GlSystem());
}

ptr<Input::Manager> Game::CreateInputManager(ptr<Window> window)
{
	BEGIN_TRY();

#if defined(___INANITY_PLATFORM_WINDOWS)
	ptr<Platform::Win32Window> win32Window = window.DynamicCast<Platform::Win32Window>();
	ptr<Input::Win32Manager> inputManager = NEW(Input::Win32RawManager(win32Window->GetHWND()));
	win32Window->SetInputManager(inputManager);
	return inputManager;
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
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
