#include "Game.hpp"
#include "../Exception.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "Win32Window.hpp"
#include "../graphics/Dx11System.hpp"
#include "../input/Win32RawManager.hpp"
#elif defined(___INANITY_PLATFORM_LINUX)
#include "X11Window.hpp"
#include "X11Display.hpp"
#include "../input/X11Manager.hpp"
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
#include "../input/EmsManager.hpp"
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
#elif defined(___INANITY_PLATFORM_LINUX)
	ptr<Platform::X11Window> x11Window = window.DynamicCast<Platform::X11Window>();
	ptr<Input::X11Manager> inputManager = NEW(Input::X11Manager(x11Window->GetDisplay()));
	x11Window->SetInputManager(inputManager);
	return inputManager;
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	return NEW(Input::EmsManager());
#else
#error Unknown platform
#endif

	END_TRY("Can't create input manager");
}

END_INANITY_PLATFORM
