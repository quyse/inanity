#include "Game.hpp"
#include "../Exception.hpp"

#ifdef ___INANITY_WINDOWS
#include "Win32Window.hpp"
#include "../graphics/Dx11System.hpp"
#include "../input/Win32RawManager.hpp"
#endif

#ifdef ___INANITY_LINUX
#include "X11Window.hpp"
#include "X11Display.hpp"
#include "../input/X11Manager.hpp"
#endif

#include "../graphics/GlSystem.hpp"

BEGIN_INANITY_PLATFORM

#ifdef ___INANITY_WINDOWS

ptr<Graphics::System> Game::CreateDx11System()
{
	return NEW(Graphics::Dx11System());
}

#endif

ptr<Graphics::System> Game::CreateGlSystem()
{
	return NEW(Graphics::GlSystem());
}

ptr<Input::Manager> Game::CreateInputManager(ptr<Window> window)
{
	BEGIN_TRY();

#ifdef ___INANITY_WINDOWS
	ptr<Platform::Win32Window> win32Window = window.DynamicCast<Platform::Win32Window>();
	ptr<Input::Win32Manager> inputManager = NEW(Input::Win32RawManager(win32Window->GetHWND()));
	win32Window->SetInputManager(inputManager);
	return inputManager;
#endif

#ifdef ___INANITY_LINUX
	ptr<Platform::X11Window> x11Window = window.DynamicCast<Platform::X11Window>();
	ptr<Input::X11Manager> inputManager = NEW(Input::X11Manager(x11Window->GetDisplay()));
	x11Window->SetInputManager(inputManager);
	return inputManager;
#endif

	END_TRY("Can't create input manager");
}

END_INANITY_PLATFORM
