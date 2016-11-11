#include "EmsWindow.hpp"
#include "Sdl.hpp"
#include "../graphics/Presenter.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../input/SdlManager.hpp"
#include "../Exception.hpp"
#include <emscripten/emscripten.h>
#include <SDL/SDL_video.h>

BEGIN_INANITY_PLATFORM

EmsWindow* EmsWindow::instance = 0;

EmsWindow::EmsWindow(const String& title, int width, int height)
: title(title), width(width), height(height)
{
	if(instance)
		THROW("Only one emscripten window is allowed");

	instance = this;

	// in emscripten all simplified
	// this is needed to initialize mouse input events
	sdl = Sdl::Get();
	SDL_SetVideoMode(0, 0, 0, SDL_OPENGL);
}

EmsWindow::~EmsWindow()
{
	instance = 0;
}

void EmsWindow::SetInputManager(ptr<Input::SdlManager> inputManager)
{
	this->inputManager = inputManager;
}

int EmsWindow::GetWidth() const
{
	return width;
}

int EmsWindow::GetHeight() const
{
	return height;
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
	emscripten_set_main_loop(&StaticMainLoop, 0, 0);
}

void EmsWindow::PlaceCursor(int x, int y)
{
	// not implemented yet
}

void EmsWindow::StartTextInput()
{
	SDL_StartTextInput();
}

void EmsWindow::StopTextInput()
{
	SDL_StopTextInput();
}

void EmsWindow::UpdateMouseLock()
{
	// not implemented yet
}

void EmsWindow::UpdateCursorVisible()
{
	// not implemented yet
}

void EmsWindow::StaticMainLoop()
{
	instance->MainLoop();
}

void EmsWindow::MainLoop()
{
	// update size
	int newWidth, newHeight, isFullScreen;
	emscripten_get_canvas_size(&newWidth, &newHeight, &isFullScreen);
	if(newWidth != width || newHeight != height)
	{
		width = newWidth;
		height = newHeight;
		if(presenter)
			presenter->Resize(width, height);
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(inputManager)
			inputManager->ProcessEvent(event);

	if(inputManager)
		inputManager->Update();
	activeHandler->Fire();
}

class EmsWindow::SdlCursor : public Window::Cursor
{
	friend class EmsWindow;
private:
	SDL_Cursor* cursor;

public:
	SdlCursor(SDL_Cursor* cursor) : cursor(cursor) {}
	~SdlCursor()
	{
		SDL_FreeCursor(cursor);
	}
};

ptr<Window::Cursor> EmsWindow::CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY)
{
	BEGIN_TRY();

	if(!(texture->GetFormat() == Graphics::PixelFormats::uintRGBA32S))
		THROW("SDL window cursor must be RGB");

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(texture->GetMipData(), texture->GetImageWidth(), texture->GetImageHeight(), 24, texture->GetMipLinePitch(), 0xFF000000, 0xFF0000, 0xFF00, 0xFF);
	if(!surface) THROW_SECONDARY("Can't create surface", Sdl::Error());

	SDL_Cursor* cursor = SDL_CreateColorCursor(surface, hotX, hotY);
	SDL_FreeSurface(surface);
	if(!cursor) THROW_SECONDARY("Can't create cursor", Sdl::Error());

	return NEW(SdlCursor(cursor));

	END_TRY("Can't create SDL cursor");
}

void EmsWindow::SetCursor(ptr<Cursor> cursor)
{
	SDL_SetCursor(cursor.FastCast<SdlCursor>()->cursor);
}

END_INANITY_PLATFORM
