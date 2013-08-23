#include "X11Manager.hpp"
#include "Frame.hpp"
#include "../platform/X11Display.hpp"

BEGIN_INANITY_INPUT

X11Manager::X11Manager(ptr<Platform::X11Display> display)
: display(display), mouseX(0), mouseY(0)
{
	int minKeycodes, maxKeycodes;
	XDisplayKeycodes(display->GetDisplay(), &minKeycodes, &maxKeycodes);
	UpdateKeyboardMapping(minKeycodes, maxKeycodes - minKeycodes + 1);
}

Key X11Manager::ConvertKey(xcb_keysym_t key)
{
#define M(xkey, key) case XK_##xkey: return Keys::key
#define M2(xkey1, xkey2, key) case XK_##xkey1: case XK_##xkey2: return Keys::key
	switch(key)
	{
		M(BackSpace, BackSpace);
		M(Tab, Tab);
		M(Linefeed, LineFeed);
		M(Escape, Escape);
		M(Delete, Delete);
		M(Home, Home);
		M(Left, Left);
		M(Up, Up);
		M(Right, Right);
		M(Down, Down);
		M(Page_Up, PageUp);
		M(Page_Down, PageDown);
		M(End, End);
		M(Begin, Begin);

		M(F1, F1);
		M(F2, F2);
		M(F3, F3);
		M(F4, F4);
		M(F5, F5);
		M(F6, F6);
		M(F7, F7);
		M(F8, F8);
		M(F9, F9);
		M(F10, F10);
		M(F11, F11);
		M(F12, F12);

		M2(a, A, A);
		M2(b, B, B);
		M2(c, C, C);
		M2(d, D, D);
		M2(e, E, E);
		M2(f, F, F);
		M2(g, G, G);
		M2(h, H, H);
		M2(i, I, I);
		M2(j, J, J);
		M2(k, K, K);
		M2(l, L, L);
		M2(m, M, M);
		M2(n, N, N);
		M2(o, O, O);
		M2(p, P, P);
		M2(q, Q, Q);
		M2(r, R, R);
		M2(s, S, S);
		M2(t, T, T);
		M2(u, U, U);
		M2(v, V, V);
		M2(w, W, W);
		M2(x, X, X);
		M2(y, Y, Y);
		M2(z, Z, Z);

#undef M
#undef M2

	default:
		return Keys::_Unknown;
	}
}

static bool ConvertButton(unsigned button, Event::Mouse::Button& outButton)
{
	switch(button)
	{
	case Button1: outButton = Event::Mouse::buttonLeft; break;
	case Button2: outButton = Event::Mouse::buttonRight; break;
	case Button3: outButton = Event::Mouse::buttonMiddle; break;
	default: return false;
	}
	return true;
}

void X11Manager::UpdateKeyboardMapping(int first, int count)
{
	int keySymsPerKeycode;

	xcb_get_keyboard_mapping_cookie_t cookie = xcb_get_keyboard_mapping(display->GetConnection(), first, count);
	xcb_generic_error_t* error;
	xcb_get_keyboard_mapping_reply_t* reply = xcb_get_keyboard_mapping_reply(display->GetConnection(), cookie, &error);
	if(error)
		display->ThrowError(error);

	xcb_keysym_t* replyKeysyms = xcb_get_keyboard_mapping_keysyms(reply);
	int replyKeysymsLength = xcb_get_keyboard_mapping_keysyms_length(reply);

	int replyKeysymsPerKeycode = reply->keysyms_per_keycode;
	int keycodesLength = replyKeysymsLength / replyKeysymsPerKeycode;
	int usefulKeysyms = replyKeysymsPerKeycode < 2 ? replyKeysymsPerKeycode : 2;

	for(int i = 0; i < keycodesLength; ++i)
		for(int j = 0; j < usefulKeysyms; ++j)
			keyboardMapping[first + i][j] = ConvertKey(replyKeysyms[i * replyKeysymsPerKeycode + j]);

	XFree(reply);
}

void X11Manager::Process(const XEvent& event)
{
	switch(event.type)
	{
	case MappingNotify: // changed keyboard mapping
		XRefreshKeyboardMapping(const_cast<XMappingEvent*>(&event.xmapping));
		UpdateKeyboardMapping(event.xmapping.first_keycode, event.xmapping.count);
		break;
	case KeyPress:
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = Event::Keyboard::typeKeyDown;
			e.keyboard.key = keyboardMapping[event.xkey.keycode][(event.xkey.state & ShiftMask) ? 1 : 0];
			AddEvent(e);
		}
		break;
	case KeyRelease:
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = Event::Keyboard::typeKeyUp;
			e.keyboard.key = keyboardMapping[event.xkey.keycode][(event.xkey.state & ShiftMask) ? 1 : 0];
			AddEvent(e);
		}
		break;
	case ButtonPress:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonDown;
			if(!ConvertButton(event.xbutton.button, e.mouse.button))
				return;
			AddEvent(e);
		}
		break;
	case ButtonRelease:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonUp;
			if(!ConvertButton(event.xbutton.button, e.mouse.button))
				return;
			AddEvent(e);
		}
		break;
	case MotionNotify:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeMove;

			{
				const State& state = internalFrame->GetCurrentState();
				e.mouse.offsetX = event.xmotion.x - mouseX;
				e.mouse.offsetY = event.xmotion.y - mouseY;
				e.mouse.offsetZ = 0;
				mouseX = event.xmotion.x;
				mouseY = event.xmotion.y;
			}
			AddEvent(e);
		}
		break;
	case EnterNotify:
		mouseX = event.xcrossing.x;
		mouseY = event.xcrossing.y;
		break;
	case LeaveNotify:
		XWarpPointer(event.xcrossing.display, 0, event.xcrossing.window, 0, 0, 0, 0, 100, 100);
		break;
	}
}

END_INANITY_INPUT
