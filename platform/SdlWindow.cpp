#include "SdlWindow.hpp"
#include "Sdl.hpp"
#include "../graphics/Presenter.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../input/SdlManager.hpp"

BEGIN_INANITY_PLATFORM

SdlWindow::SdlWindow(SDL_Window* handle)
: sdl(Sdl::Get()), handle(handle), quit(false), fullscreen(false)
{
	SDL_GetWindowSize(handle, &virtualWidth, &virtualHeight);
	SDL_GL_GetDrawableSize(handle, &clientWidth, &clientHeight);
}

SdlWindow::~SdlWindow()
{
	Close();
}

SDL_Window* SdlWindow::GetHandle() const
{
	return handle;
}

void SdlWindow::SetInputManager(ptr<Input::SdlManager> inputManager)
{
	this->inputManager = inputManager;
	inputManager->SetVirtualScale((float)clientWidth / (float)virtualWidth, (float)clientHeight / (float)virtualHeight);
}

int SdlWindow::GetClientWidth() const
{
	return clientWidth;
}

int SdlWindow::GetClientHeight() const
{
	return clientHeight;
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

void SdlWindow::SetFullScreen(bool fullscreen)
{
	if(this->fullscreen == fullscreen) return;
	this->fullscreen = fullscreen;
	SDL_SetWindowFullscreen(handle, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void SdlWindow::GetRect(int& left, int& top, int& width, int& height)
{
	SDL_GetWindowPosition(handle, &left, &top);
	SDL_GetWindowSize(handle, &width, &height);
	int borderTop, borderLeft, borderBottom, borderRight;
	SDL_GetWindowBordersSize(handle, &borderTop, &borderLeft, &borderBottom, &borderRight);
	left -= borderLeft;
	top -= borderTop;
	width += borderLeft + borderRight;
	height += borderTop + borderBottom;
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
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SDL_GetWindowSize(handle, &virtualWidth, &virtualHeight);
					SDL_GL_GetDrawableSize(handle, &clientWidth, &clientHeight);
					if(inputManager)
						inputManager->SetVirtualScale((float)clientWidth / (float)virtualWidth, (float)clientHeight / (float)virtualHeight);
					if(presenter)
						presenter->Resize(clientWidth, clientHeight);
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

void SdlWindow::PlaceCursor(int x, int y)
{
	SDL_WarpMouseInWindow(handle, x, y);
}

void SdlWindow::StartTextInput()
{
	SDL_StartTextInput();
}

void SdlWindow::StopTextInput()
{
	SDL_StopTextInput();
}

void SdlWindow::UpdateMouseLock()
{
	SDL_SetRelativeMouseMode(mouseLock ? SDL_TRUE : SDL_FALSE);
}

void SdlWindow::UpdateCursorVisible()
{
	SDL_ShowCursor(cursorVisible ? SDL_ENABLE : SDL_DISABLE);
}

SDL_Surface* SdlWindow::CreateSurfaceFromTexture(ptr<Graphics::RawTextureData> texture)
{
	texture = texture->PremultiplyAlpha();

	int width = texture->GetImageWidth();
	int height = texture->GetImageHeight();
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	if(!surface) THROW_SECONDARY("Can't create surface", Sdl::Error());

	SDL_LockSurface(surface);
	void* texturePixels = texture->GetMipData();
	int texturePitch = texture->GetMipLinePitch();
	for(int i = 0; i < height; ++i)
		memcpy((uint8_t*)surface->pixels + i * surface->pitch, (uint8_t*)texturePixels + i * texturePitch, width * 4);
	SDL_UnlockSurface(surface);

	return surface;
}

class SdlWindow::SdlIcon : public Window::Icon
{
	friend class SdlWindow;
private:
	SDL_Surface* surface;

public:
	SdlIcon(SDL_Surface* surface) : surface(surface) {}
	~SdlIcon()
	{
		SDL_FreeSurface(surface);
	}
};

ptr<Window::Icon> SdlWindow::CreateIcon(ptr<Graphics::RawTextureData> texture)
{
	BEGIN_TRY();

	if(!(texture->GetFormat() == Graphics::PixelFormats::uintRGBA32S))
		THROW("SDL window icon must be RGB");

	return NEW(SdlIcon(CreateSurfaceFromTexture(texture)));

	END_TRY("Can't create SDL icon");
}

void SdlWindow::SetIcon(ptr<Icon> icon)
{
	SDL_SetWindowIcon(handle, icon.FastCast<SdlIcon>()->surface);
}

class SdlWindow::SdlCursor : public Window::Cursor
{
	friend class SdlWindow;
private:
	SDL_Cursor* cursor;

public:
	SdlCursor(SDL_Cursor* cursor) : cursor(cursor) {}
	~SdlCursor()
	{
		SDL_FreeCursor(cursor);
	}
};

ptr<Window::Cursor> SdlWindow::CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY)
{
	BEGIN_TRY();

	if(!(texture->GetFormat() == Graphics::PixelFormats::uintRGBA32S))
		THROW("SDL window cursor must be RGB");

	SDL_Surface* surface = CreateSurfaceFromTexture(texture);
	SDL_Cursor* cursor = SDL_CreateColorCursor(surface, hotX, hotY);
	SDL_FreeSurface(surface);
	if(!cursor) THROW_SECONDARY("Can't create cursor", Sdl::Error());

	return NEW(SdlCursor(cursor));

	END_TRY("Can't create SDL cursor");
}

void SdlWindow::SetCursor(ptr<Cursor> cursor)
{
	SDL_SetCursor(cursor.FastCast<SdlCursor>()->cursor);
}

END_INANITY_PLATFORM
