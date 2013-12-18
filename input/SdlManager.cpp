#include "SdlManager.hpp"
#include "Frame.hpp"
#include "../platform/Sdl.hpp"
#include <SDL.h>

BEGIN_INANITY_INPUT

SdlManager::SdlManager()
{
	sdl = Platform::Sdl::Get();
	//SDL_InitSubSystem(SDL_INIT_EVENTS);
}

SdlManager::~SdlManager()
{
	//SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

static Key ConvertKey(SDL_Scancode code)
{
	Key key;
	switch(code)
	{
#define C(c, k) case SDL_SCANCODE_##c: key = Key::k; break

		C(0, _0);
		C(1, _1);
		C(2, _2);
		C(3, _3);
		C(4, _4);
		C(5, _5);
		C(6, _6);
		C(7, _7);
		C(8, _8);
		C(9, _9);

		C(A, A);
		C(B, B);
		C(C, C);
		C(D, D);
		C(E, E);
		C(F, F);
		C(G, G);
		C(H, H);
		C(I, I);
		C(J, J);
		C(K, K);
		C(L, L);
		C(M, M);
		C(N, N);
		C(O, O);
		C(P, P);
		C(Q, Q);
		C(R, R);
		C(S, S);
		C(T, T);
		C(U, U);
		C(V, V);
		C(W, W);
		C(X, X);
		C(Y, Y);
		C(Z, Z);

#undef C

	default:
		key = Key::_Unknown;
		break;
	}

	return key;
}

void SdlManager::Update()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			{
				Event e;
				e.device = Event::deviceKeyboard;
				e.keyboard.type = event.type == SDL_KEYDOWN ? Event::Keyboard::typeKeyDown : Event::Keyboard::typeKeyUp;
				e.keyboard.key = ConvertKey(event.key.keysym.scancode);
				AddEvent(e);
			}
			break;
		case SDL_MOUSEMOTION:
			{
				Event e;
				e.device = Event::deviceMouse;
				e.mouse.type = Event::Mouse::typeRawMove;
				e.mouse.rawMoveX = event.motion.xrel;
				e.mouse.rawMoveY = event.motion.yrel;
				e.mouse.rawMoveZ = 0;
				AddEvent(e);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			{
				Event e;
				e.device = Event::deviceMouse;
				e.mouse.type = event.type == SDL_MOUSEBUTTONDOWN ? Event::Mouse::typeButtonDown : Event::Mouse::typeButtonUp;
				bool ok = true;
				switch(event.button.button)
				{
				case SDL_BUTTON_LEFT:
					e.mouse.button = Event::Mouse::buttonLeft;
					break;
				case SDL_BUTTON_MIDDLE:
					e.mouse.button = Event::Mouse::buttonMiddle;
					break;
				case SDL_BUTTON_RIGHT:
					e.mouse.button = Event::Mouse::buttonRight;
					break;
				default:
					ok = false;
					break;
				}
				if(ok)
					AddEvent(e);
			}
			break;
		}
	}

	Manager::Update();
}

END_INANITY_INPUT
