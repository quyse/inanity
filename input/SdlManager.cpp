#include "SdlManager.hpp"
#include "Frame.hpp"
#include "../platform/Sdl.hpp"

BEGIN_INANITY_INPUT

SdlManager::SdlManager() : sdl(Platform::Sdl::Get()) {}

static Key ConvertKey(SDL_Keycode code)
{
	Key key;
	switch(code)
	{
#define C(c, k) case SDLK_##c: key = Key::k; break

		C(BACKSPACE, BackSpace);
		C(TAB, Tab);
		C(CLEAR, Clear);
		C(RETURN, Return);
		C(PAUSE, Pause);
		C(SCROLLLOCK, ScrollLock);
		C(SYSREQ, SysReq);
		C(ESCAPE, Escape);
		C(INSERT, Insert);
		C(DELETE, Delete);

		C(SPACE, Space);

		C(PLUS, Plus);
		C(COMMA, Comma);
		C(MINUS, Hyphen);
		C(PERIOD, Period);
		C(SLASH, Slash);

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

		C(a, A);
		C(b, B);
		C(c, C);
		C(d, D);
		C(e, E);
		C(f, F);
		C(g, G);
		C(h, H);
		C(i, I);
		C(j, J);
		C(k, K);
		C(l, L);
		C(m, M);
		C(n, N);
		C(o, O);
		C(p, P);
		C(q, Q);
		C(r, R);
		C(s, S);
		C(t, T);
		C(u, U);
		C(v, V);
		C(w, W);
		C(x, X);
		C(y, Y);
		C(z, Z);

		C(HOME, Home);
		C(LEFT, Left);
		C(UP, Up);
		C(RIGHT, Right);
		C(DOWN, Down);
		C(PAGEUP, PageUp);
		C(PAGEDOWN, PageDown);
		C(END, End);
		C(BACKQUOTE, Grave);

		C(KP_SPACE, NumPadSpace);
		C(KP_TAB, NumPadTab);
		C(KP_ENTER, NumPadEnter);
		C(KP_EQUALS, NumPadEqual);
		C(KP_MULTIPLY, NumPadMultiply);
		C(KP_PLUS, NumPadAdd);
		C(KP_MINUS, NumPadSubtract);
		C(KP_DECIMAL, NumPadDecimal);
		C(KP_DIVIDE, NumPadDivide);

		C(KP_0, NumPad0);
		C(KP_1, NumPad1);
		C(KP_2, NumPad2);
		C(KP_3, NumPad3);
		C(KP_4, NumPad4);
		C(KP_5, NumPad5);
		C(KP_6, NumPad6);
		C(KP_7, NumPad7);
		C(KP_8, NumPad8);
		C(KP_9, NumPad9);

		C(F1, F1);
		C(F2, F2);
		C(F3, F3);
		C(F4, F4);
		C(F5, F5);
		C(F6, F6);
		C(F7, F7);
		C(F8, F8);
		C(F9, F9);
		C(F10, F10);
		C(F11, F11);
		C(F12, F12);

		C(LSHIFT, ShiftL);
		C(RSHIFT, ShiftR);
		C(LCTRL, ControlL);
		C(RCTRL, ControlR);
		C(CAPSLOCK, CapsLock);
		C(LGUI, SuperL);
		C(RGUI, SuperR);
		C(LALT, AltL);
		C(RALT, AltR);

#undef C

	default:
		key = Key::_Unknown;
		break;
	}

	return key;
}

void SdlManager::ProcessEvent(const SDL_Event& event)
{
	switch(event.type)
	{
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = event.type == SDL_KEYDOWN ? Event::Keyboard::typeKeyDown : Event::Keyboard::typeKeyUp;
			e.keyboard.key = ConvertKey(event.key.keysym.sym);
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

END_INANITY_INPUT
