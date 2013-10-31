#include "EmsWindow.hpp"
#include "../graphics/EmsOutput.hpp"
#include "../Exception.hpp"
#include <emscripten/emscripten.h>

BEGIN_INANITY_PLATFORM

EmsWindow* EmsWindow::instance = 0;

EmsWindow::EmsWindow(const String& title, int width, int height)
: title(title)
{
	if(instance)
		THROW("Only one emscripten window is allowed");

	instance = this;

	// init canvas
	emscripten_set_canvas_size(width, height);
}

EmsWindow::~EmsWindow()
{
	instance = 0;
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
	instance->activeHandler->Fire();
}

END_INANITY_PLATFORM
