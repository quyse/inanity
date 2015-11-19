#include "Sdl.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

Sdl* Sdl::instance = nullptr;
Uint32 Sdl::initializedSubsystems = 0;

Sdl::Sdl()
{
	instance = this;
	initializedSubsystems = 0;
	SDL_Init(0);
}

Sdl::~Sdl()
{
	instance = nullptr;
	if(initializedSubsystems)
	{
		SDL_QuitSubSystem(initializedSubsystems);
		initializedSubsystems = 0;
	}
	SDL_Quit();
}

ptr<Sdl> Sdl::Get(Uint32 flags)
{
	if(!instance)
		instance = NEW(Sdl());

	flags &= ~initializedSubsystems;
	if(flags)
	{
		if(SDL_InitSubSystem(flags) != 0)
			THROW_SECONDARY("Can't initialize SDL subsystem(s)", Error());
		initializedSubsystems |= flags;

		// set some additional flags for video subsystem
		// hacky: better to place this code somewhere into GlDevice or GlSystem
		if(flags & SDL_INIT_VIDEO)
		{
			struct
			{
				SDL_GLattr attr;
				int value;
			} attrs[] =
			{
				{ SDL_GL_CONTEXT_MAJOR_VERSION, 3 },
				{ SDL_GL_CONTEXT_MINOR_VERSION, 3 },
				{ SDL_GL_DOUBLEBUFFER, 1 },
				{ SDL_GL_RED_SIZE, 8 },
				{ SDL_GL_GREEN_SIZE, 8 },
				{ SDL_GL_BLUE_SIZE, 8 },
				{ SDL_GL_DEPTH_SIZE, 0 },
				{ SDL_GL_STENCIL_SIZE, 0 }
			};
			for(size_t i = 0; i < sizeof(attrs) / sizeof(attrs[0]); ++i)
				if(SDL_GL_SetAttribute(attrs[i].attr, attrs[i].value) != 0)
					THROW("Can't set SDL video subsystem attribute");
		}
	}

	return instance;
}

ptr<Exception> Sdl::Error()
{
	const char* errorString = SDL_GetError();
	if(!errorString || !*errorString)
		return nullptr;
	return NEW(Exception(errorString));
}

END_INANITY_PLATFORM
