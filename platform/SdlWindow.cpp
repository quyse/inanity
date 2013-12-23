#include "SdlWindow.hpp"
#include "Sdl.hpp"
#include "../input/SdlManager.hpp"
#include "../graphics/WindowOutput.hpp"

BEGIN_INANITY_PLATFORM

SdlWindow::SdlWindow(SDL_Window* handle)
: sdl(Sdl::Get()), handle(handle), quit(false) {}

SdlWindow::~SdlWindow()
{
	Close();
}

SDL_Window* SdlWindow::GetHandle() const
{
	return handle;
}

void SdlWindow::SetOutput(ptr<Graphics::Output> output)
{
	this->output = output;
}

void SdlWindow::SetInputManager(ptr<Input::SdlManager> inputManager)
{
	this->inputManager = inputManager;
}

void SdlWindow::SetTitle(const String& title)
{
	SDL_SetWindowTitle(handle, title.c_str());
}

void SdlWindow::Close()
{
	if(handle)
	{
		SDL_DestroyWindow(handle);
		handle = nullptr;
	}
	quit = true;
}

void SdlWindow::Run(ptr<Handler> activeHandler)
{
	while(!quit)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(inputManager)
				inputManager->ProcessEvent(event);

			if(event.type == SDL_WINDOWEVENT)
			{
				switch(event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					if(output)
						output->Resize(event.window.data1, event.window.data2);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					Close();
					break;
				}
			}
		}

		if(inputManager)
			inputManager->Update();
		activeHandler->Fire();
	}
}

ptr<Graphics::Output> SdlWindow::CreateOutput()
{
	// get client size of the window
	int width, height;
	SDL_GetWindowSize(handle, &width, &height);
	return NEW(Graphics::WindowOutput(this, width, height));
}

void SdlWindow::PlaceCursor(int x, int y)
{
	SDL_WarpMouseInWindow(handle, x, y);
}

void SdlWindow::UpdateMouseLock()
{
	SDL_SetRelativeMouseMode(mouseLock ? SDL_TRUE : SDL_FALSE);
}

void SdlWindow::UpdateCursorVisible()
{
	// not supported by SDL directly, but can be done with cursors
}

END_INANITY_PLATFORM
