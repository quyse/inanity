#include "CoreManager.hpp"
#include "../platform/CoreWindow.hpp"
#include "Controller.hpp"
#include "Frame.hpp"
#include <unordered_map>

using Windows::Foundation::Collections::IVectorView;
using Windows::Foundation::EventHandler;
using Windows::Xbox::Input::Gamepad;
using Windows::Xbox::Input::GamepadAddedEventArgs;
using Windows::Xbox::Input::GamepadButtons;
using Windows::Xbox::Input::GamepadRemovedEventArgs;
using Windows::Xbox::Input::GamepadVibration;
using Windows::Xbox::Input::IGamepad;
using Windows::Xbox::Input::RawGamepadReading;

BEGIN_INANITY_INPUT

ref class CoreManager::Impl sealed
{
private:
	CoreManager* manager;

	struct CoreController : public Controller
	{
		IGamepad^ gamepad;
		RawGamepadReading lastReading;

		CoreController(IGamepad^ gamepad) : Controller(gamepad->Id), gamepad(gamepad)
		{
			lastReading = gamepad->GetRawCurrentReading();
		}

		void Reset()
		{
			gamepad = nullptr;
		}

		// Controller's methods
		bool IsActive() const
		{
			return gamepad != nullptr;
		}

		void RunHapticLeftRight(float left, float right)
		{
			if(gamepad)
			{
				GamepadVibration vibration;
				vibration.LeftMotorLevel = left;
				vibration.RightMotorLevel = right;
				vibration.LeftTriggerLevel = 0;
				vibration.RightTriggerLevel = 0;
				gamepad->SetVibration(vibration);
			}
		}

		void StopHaptic()
		{
			RunHapticLeftRight(0, 0);
		}
	};

	std::unordered_map<uint64_t, ptr<CoreController> > controllers;

internal:
	Impl(CoreManager* manager) : manager(manager)
	{
		// add gamepads initially
		{
			IVectorView<IGamepad^>^ gamepads = Gamepad::Gamepads;
			for(uint32 i = 0; i < gamepads->Size; ++i)
				AddGamepad(gamepads->GetAt(i));
		}

		Gamepad::GamepadAdded += ref new EventHandler<GamepadAddedEventArgs^>(this, &Impl::OnGamepadAdded);
		Gamepad::GamepadRemoved += ref new EventHandler<GamepadRemovedEventArgs^>(this, &Impl::OnGamepadRemoved);
	}

	void Reset()
	{
		manager = nullptr;
	}

	void Update()
	{
		for(auto& controller : controllers)
		{
			RawGamepadReading reading = controller.second->gamepad->GetRawCurrentReading();

			if(manager)
			{
				// buttons
				auto buttonEvent = [&](GamepadButtons button, Event::Controller::Button ebutton)
				{
					if((reading.Buttons & button) != (controller.second->lastReading.Buttons & button))
					{
						Event e;
						e.device = Event::deviceController;
						e.controller.type = (reading.Buttons & button) != GamepadButtons::None ? Event::Controller::typeButtonDown : Event::Controller::typeButtonUp;
						e.controller.device = controller.first;
						e.controller.button = ebutton;
						manager->AddEvent(e);
					}
				};

#define BUTTON_EVENT(b, eb) buttonEvent(GamepadButtons::b, Event::Controller::Button::button##eb)

				BUTTON_EVENT(A, A);
				BUTTON_EVENT(B, B);
				BUTTON_EVENT(X, X);
				BUTTON_EVENT(Y, Y);
				BUTTON_EVENT(View, Back);
				BUTTON_EVENT(Menu, Start);
				BUTTON_EVENT(LeftThumbstick, LeftStick);
				BUTTON_EVENT(RightThumbstick, RightStick);
				BUTTON_EVENT(LeftShoulder, LeftShoulder);
				BUTTON_EVENT(RightShoulder, RightShoulder);
				BUTTON_EVENT(DPadUp, DPadUp);
				BUTTON_EVENT(DPadDown, DPadDown);
				BUTTON_EVENT(DPadLeft, DPadLeft);
				BUTTON_EVENT(DPadRight, DPadRight);

#undef BUTTON_EVENT

				// axes
				auto axisEvent = [&](float32 RawGamepadReading::* axis, Event::Controller::Axis eaxis, int multiplier)
				{
					if(reading.*axis != controller.second->lastReading.*axis)
					{
						Event e;
						e.device = Event::deviceController;
						e.controller.type = Event::Controller::typeAxisMotion;
						e.controller.device = controller.first;
						e.controller.axis = eaxis;
						e.controller.axisValue = int(reading.*axis * multiplier);
						manager->AddEvent(e);
					}
				};

#define AXIS_EVENT(a, ea, m) axisEvent(&RawGamepadReading::a, Event::Controller::Axis::axis##ea, m)

				AXIS_EVENT(LeftThumbstickX, LeftX, 32767);
				AXIS_EVENT(LeftThumbstickY, LeftY, -32767);
				AXIS_EVENT(RightThumbstickX, RightX, 32767);
				AXIS_EVENT(RightThumbstickY, RightY, -32767);
				AXIS_EVENT(LeftTrigger, TriggerLeft, 255);
				AXIS_EVENT(RightTrigger, TriggerRight, 255);

#undef AXIS_EVENT
			}

			controller.second->lastReading = reading;
		}
	}

	ptr<Controller> TryGetController(uint64_t controllerId)
	{
		auto i = controllers.find(controllerId);
		return i != controllers.end() ? i->second : nullptr;
	}

private:
	void OnGamepadAdded(::Platform::Object^ sender, GamepadAddedEventArgs^ args)
	{
		AddGamepad(args->Gamepad);
	}

	void OnGamepadRemoved(::Platform::Object^ sender, GamepadRemovedEventArgs^ args)
	{
		RemoveGamepad(args->Gamepad->Id);
	}

	void AddGamepad(IGamepad^ gamepad)
	{
		controllers.insert(std::make_pair(gamepad->Id, NEW(CoreController(gamepad))));

		if(manager)
		{
			Event e;
			e.device = Event::deviceController;
			e.controller.type = Event::Controller::typeDeviceAdded;
			e.controller.device = gamepad->Id;
			manager->AddEvent(e);
		}
	}

	void RemoveGamepad(uint64_t gamepadId)
	{
		{
			auto i = controllers.find(gamepadId);
			if(i != controllers.end())
			{
				i->second->Reset();
				controllers.erase(i);
			}
		}

		if(manager)
		{
			Event e;
			e.device = Event::deviceController;
			e.controller.type = Event::Controller::typeDeviceRemoved;
			e.controller.device = gamepadId;
			manager->AddEvent(e);
		}
	}
};

CoreManager::CoreManager(ptr<Platform::CoreWindow> window)
{
	impl = ref new Impl(this);
}

CoreManager::~CoreManager()
{
	impl->Reset();
}

void CoreManager::Update()
{
	impl->Update();

	Manager::Update();
}

ptr<Controller> CoreManager::TryGetController(uint64_t controllerId)
{
	return impl->TryGetController(controllerId);
}

END_INANITY_INPUT
