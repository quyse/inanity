#include "SdlManager.hpp"
#include "Frame.hpp"
#include "../platform/Sdl.hpp"
#include "../CriticalCode.hpp"
#include "../deps/utf8.h"

BEGIN_INANITY_INPUT

SdlManager::SdlManager()
: sdl(Platform::Sdl::Get(
#if defined(___INANITY_PLATFORM_EMSCRIPTEN)
	0
#else
	SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC
#endif
	)), widthScale(1), heightScale(1)
{
}

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
	case SDL_TEXTINPUT:
		for(auto i = utf8::unchecked::iterator<const char*>(event.text.text); *i.base(); ++i)
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = Event::Keyboard::typeCharacter;
			e.keyboard.character = *i;
			AddEvent(e);
		}
		break;
	case SDL_MOUSEMOTION:
		// raw move event
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeRawMove;
			e.mouse.rawMoveX = event.motion.xrel;
			e.mouse.rawMoveY = event.motion.yrel;
			e.mouse.rawMoveZ = 0;
			AddEvent(e);
		}
		// cursor move event
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeCursorMove;
			e.mouse.cursorX = (int)(event.motion.x * widthScale);
			e.mouse.cursorY = (int)(event.motion.y * heightScale);
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
			{
				AddEvent(e);
				// send double click event for second button up
#if !defined(___INANITY_PLATFORM_EMSCRIPTEN)
				if(e.mouse.type == Event::Mouse::typeButtonUp && event.button.clicks == 2)
				{
					e.mouse.type = Event::Mouse::typeDoubleClick;
					AddEvent(e);
				}
#endif
			}
		}
		break;
#if !defined(___INANITY_PLATFORM_EMSCRIPTEN)
	case SDL_CONTROLLERDEVICEADDED:
		// device added event
		{
			// open controller
			SDL_GameController* sdlController = SDL_GameControllerOpen(event.cdevice.which);
			if(sdlController)
			{
				ptr<SdlController> controller = NEW(SdlController(sdlController));
				{
					CriticalCode criticalCode(criticalSection);
					controllers[controller->GetControllerId()] = controller;
				}

				Event e;
				e.device = Event::deviceController;
				e.controller.type = Event::Controller::typeDeviceAdded;
				e.controller.device = controller->GetControllerId();
				AddEvent(e);
			}
		}
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		// device removed event
		{
			// try to find controller by id, and remove it from map
			ptr<SdlController> controller;
			{
				CriticalCode criticalCode(criticalSection);
				decltype(controllers)::const_iterator i = controllers.find(event.cdevice.which);
				if(i != controllers.end())
				{
					controller = i->second;
					controllers.erase(i);
				}
			}

			// if it's found
			if(controller)
			{
				// close controller
				controller->Close();
				controller = nullptr;

				// emit event
				Event e;
				e.device = Event::deviceController;
				e.controller.type = Event::Controller::typeDeviceRemoved;
				e.controller.device = event.cdevice.which;
				AddEvent(e);
			}
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
		// controller button down/up event
		{
			Event::Controller::Button button;
			bool ok = true;
			switch(event.cbutton.button)
			{
#define B(a, b) case SDL_CONTROLLER_BUTTON_##a: button = Event::Controller::button##b; break
			B(A, A);
			B(B, B);
			B(X, X);
			B(Y, Y);
			B(BACK, Back);
			B(GUIDE, Guide);
			B(START, Start);
			B(LEFTSTICK, LeftStick);
			B(RIGHTSTICK, RightStick);
			B(LEFTSHOULDER, LeftShoulder);
			B(RIGHTSHOULDER, RightShoulder);
			B(DPAD_UP, DPadUp);
			B(DPAD_DOWN, DPadDown);
			B(DPAD_LEFT, DPadLeft);
			B(DPAD_RIGHT, DPadRight);
#undef B
			default: ok = false; break;
			}
			if(ok)
			{
				Event e;
				e.device = Event::deviceController;
				e.controller.type = event.type == SDL_CONTROLLERBUTTONDOWN ? Event::Controller::typeButtonDown : Event::Controller::typeButtonUp;
				e.controller.device = event.cbutton.which;
				e.controller.button = button;
				AddEvent(e);
			}
		}
		break;
	case SDL_CONTROLLERAXISMOTION:
		// controller axis motion event
		{
			Event::Controller::Axis axis;
			bool ok = true;
			switch(event.caxis.axis)
			{
#define A(a, b) case SDL_CONTROLLER_AXIS_##a: axis = Event::Controller::axis##b; break
			A(LEFTX, LeftX);
			A(LEFTY, LeftY);
			A(RIGHTX, RightX);
			A(RIGHTY, RightY);
			A(TRIGGERLEFT, TriggerLeft);
			A(TRIGGERRIGHT, TriggerRight);
#undef A
			default: ok = false; break;
			}
			if(ok)
			{
				Event e;
				e.device = Event::deviceController;
				e.controller.type = Event::Controller::typeAxisMotion;
				e.controller.device = event.caxis.which;
				e.controller.axis = axis;
				e.controller.axisValue = event.caxis.value;
				AddEvent(e);
			}
		}
		break;
#endif // !defined(___INANITY_PLATFORM_EMSCRIPTEN)
	}
}

void SdlManager::SetVirtualScale(float widthScale, float heightScale)
{
	this->widthScale = widthScale;
	this->heightScale = heightScale;
}

void SdlManager::StartTextInput()
{
	Manager::StartTextInput();
	SDL_StartTextInput();
}

void SdlManager::StopTextInput()
{
	Manager::StopTextInput();
	SDL_StopTextInput();
}

#if !defined(___INANITY_PLATFORM_EMSCRIPTEN)

ptr<Controller> SdlManager::TryGetController(int controllerId)
{
	CriticalCode criticalCode(criticalSection);

	decltype(controllers)::const_iterator i = controllers.find(controllerId);
	if(i != controllers.end())
		return i->second;
	return nullptr;
}

void SdlManager::TryLoadControllerMappingsFromFile(const char* fileName)
{
	SDL_GameControllerAddMappingsFromFile(fileName);
}

SdlManager::SdlController::SdlController(SDL_GameController* controller, SDL_Joystick* joystick)
: Controller(SDL_JoystickInstanceID(joystick))
, controller(controller)
, haptic(SDL_HapticOpenFromJoystick(joystick))
, hapticEffectIndex(-1)
{}

SdlManager::SdlController::SdlController(SDL_GameController* controller)
: SdlController(controller, SDL_GameControllerGetJoystick(controller))
{}

SdlManager::SdlController::~SdlController()
{
	Close();
}

bool SdlManager::SdlController::IsActive() const
{
	CriticalCode criticalCode(criticalSection);

	return controller;
}

void SdlManager::SdlController::RunHapticLeftRight(float left, float right)
{
	CriticalCode criticalCode(criticalSection);

	if(!haptic) return;

	hapticEffect.leftright.length = SDL_HAPTIC_INFINITY;
	hapticEffect.leftright.large_magnitude = uint16_t(left * 0xffff);
	hapticEffect.leftright.small_magnitude = uint16_t(right * 0xffff);

	if(hapticEffectIndex >= 0 && hapticEffect.type == SDL_HAPTIC_LEFTRIGHT)
	{
		SDL_HapticStopEffect(haptic, hapticEffectIndex);
		SDL_HapticUpdateEffect(haptic, hapticEffectIndex, &hapticEffect);
	}
	else
	{
		if(hapticEffectIndex >= 0)
		{
			SDL_HapticStopEffect(haptic, hapticEffectIndex);
			SDL_HapticDestroyEffect(haptic, hapticEffectIndex);
		}

		hapticEffect.type = SDL_HAPTIC_LEFTRIGHT;
		hapticEffectIndex = SDL_HapticNewEffect(haptic, &hapticEffect);
	}

	SDL_HapticRunEffect(haptic, hapticEffectIndex, 1);
}

void SdlManager::SdlController::StopHaptic()
{
	CriticalCode criticalCode(criticalSection);

	if(hapticEffectIndex >= 0)
	{
		SDL_HapticStopEffect(haptic, hapticEffectIndex);
	}
}

void SdlManager::SdlController::Close()
{
	CriticalCode criticalCode(criticalSection);

	if(hapticEffectIndex >= 0)
	{
		SDL_HapticStopEffect(haptic, hapticEffectIndex);
		SDL_HapticDestroyEffect(haptic, hapticEffectIndex);
		hapticEffectIndex = -1;
	}

	if(haptic)
	{
		SDL_HapticClose(haptic);
		haptic = nullptr;
	}

	if(controller)
	{
		SDL_GameControllerClose(controller);
		controller = nullptr;
	}
}

#endif

END_INANITY_INPUT
