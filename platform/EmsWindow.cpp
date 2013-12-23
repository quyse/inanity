#include "EmsWindow.hpp"
#include "Sdl.hpp"
#include "../graphics/EmsOutput.hpp"
#include "../input/SdlManager.hpp"
#include "../Exception.hpp"
#include <emscripten/emscripten.h>
#include <SDL/SDL_video.h>

BEGIN_INANITY_PLATFORM

EmsWindow* EmsWindow::instance = 0;

EmsWindow::EmsWindow(const String& title, int width, int height)
: title(title)
{
	if(instance)
		THROW("Only one emscripten window is allowed");

	instance = this;

	// in emscripten all simplified
	// this is needed to initialize mouse input events
	sdl = Sdl::Get();
	SDL_SetVideoMode(0, 0, 0, SDL_OPENGL);

	// init canvas
	emscripten_set_canvas_size(width, height);
}

EmsWindow::~EmsWindow()
{
	instance = 0;
}

void EmsWindow::SetInputManager(ptr<Input::SdlManager> inputManager)
{
	this->inputManager = inputManager;
}

void EmsWindow::SetTitle(const String& title)
{
	this->title = title;
}

void EmsWindow::Close()
{
	emscripten_cancel_main_loop();
}

void EmsWindow::Run(ptr<Handler> activeHandler)
{
	this->activeHandler = activeHandler;
	emscripten_set_main_loop(&MainLoop, 0, 0);
}

ptr<Graphics::Output> EmsWindow::CreateOutput()
{
	return NEW(Graphics::EmsOutput());
}

void EmsWindow::PlaceCursor(int x, int y)
{
	// not implemented yet
}

void EmsWindow::UpdateMouseLock()
{
	// not implemented yet
}

void EmsWindow::UpdateCursorVisible()
{
	// not implemented yet
}

void EmsWindow::MainLoop()
{
	ptr<Input::SdlManager> inputManager = instance->inputManager;

	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(inputManager)
			inputManager->ProcessEvent(event);

	if(inputManager)
		inputManager->Update();
	instance->activeHandler->Fire();
}

END_INANITY_PLATFORM
