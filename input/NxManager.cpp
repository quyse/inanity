#include "NxManager.hpp"
#include "Frame.hpp"
#include "../platform/NxWindow.hpp"

BEGIN_INANITY_INPUT

NxManager::NxController::NxController(uint64_t controllerId)
: Controller(controllerId)
{
	// initialize vibration devices
	nn::hid::VibrationDeviceHandle vibrationDevices[2];
	int vibrationDevicesCount = nn::hid::GetVibrationDeviceHandles(vibrationDevices, sizeof(vibrationDevices) / sizeof(vibrationDevices[0]), controllerId, nn::hid::NpadStyleFullKey::Mask);
	for(int i = 0; i < vibrationDevicesCount; ++i)
	{
		const nn::hid::VibrationDeviceHandle& vibrationDevice = vibrationDevices[i];
		nn::hid::VibrationDeviceInfo vibrationDeviceInfo;
		nn::hid::GetVibrationDeviceInfo(&vibrationDeviceInfo, vibrationDevice);
		switch(vibrationDeviceInfo.position)
		{
		case nn::hid::VibrationDevicePosition_Left:
			leftVibrationDevice = vibrationDevice;
			break;
		case nn::hid::VibrationDevicePosition_Right:
			rightVibrationDevice = vibrationDevice;
			break;
		default:
			continue;
		}

		nn::hid::InitializeVibrationDevice(vibrationDevice);
	}
}

bool NxManager::NxController::IsActive() const
{
	return active;
}

void NxManager::NxController::RunHapticLeftRight(float left, float right)
{
	// treat "left" and "right" values as low and high amplitudes
	// for compatibility with Xbox controller
	nn::hid::VibrationValue value;
	value.frequencyLow = nn::hid::VibrationFrequencyLowDefault;
	value.frequencyHigh = nn::hid::VibrationFrequencyHighDefault;
	value.amplitudeLow = right;
	value.amplitudeHigh = left;

	if(leftVibrationDevice._storage)
	{
		// value.amplitudeLow = value.amplitudeHigh = left * 0.5f;
		nn::hid::SendVibrationValue(leftVibrationDevice, value);
	}

	if(rightVibrationDevice._storage)
	{
		// value.amplitudeLow = value.amplitudeHigh = right * 0.5f;
		nn::hid::SendVibrationValue(rightVibrationDevice, value);
	}
}

void NxManager::NxController::StopHaptic()
{
	RunHapticLeftRight(0, 0);
}

NxManager::NxManager()
{
	// initialize controllers
	nn::hid::InitializeNpad();
	nn::hid::SetSupportedNpadStyleSet(nn::hid::NpadStyleFullKey::Mask | nn::hid::NpadStyleHandheld::Mask);

	const nn::hid::NpadIdType npadIds[] = {
		nn::hid::NpadId::No1,
		nn::hid::NpadId::No2,
		nn::hid::NpadId::No3,
		nn::hid::NpadId::No4,
		nn::hid::NpadId::Handheld
	};
	const size_t npadsCount = sizeof(npadIds) / sizeof(npadIds[0]);
	nn::hid::SetSupportedNpadIdType(npadIds, npadsCount);

	for(size_t i = 0; i < npadsCount; ++i)
		controllers.insert({ (uint64_t)npadIds[i], { -1, nullptr } });
}

void NxManager::Update()
{
	for(Controllers::iterator i = controllers.begin(); i != controllers.end(); ++i)
	{
		uint64_t controllerId = i->first;
		int64_t& lastSamplingNumber = i->second.first;
		ptr<NxController>& controller = i->second.second;

		auto processStates = [&](const auto* npadStates, int statesRead)
		{
			for(int j = statesRead - 1; j >= 0; --j)
			{
				const auto& npadState = npadStates[j];

				// skip already processed states
				if(npadState.samplingNumber <= lastSamplingNumber) continue;
				lastSamplingNumber = npadState.samplingNumber;

				// process connectivity
				bool isConnected = npadState.attributes.template Test<nn::hid::NpadAttribute::IsConnected>();
				if(isConnected != !!controller)
				{
					if(isConnected)
					{
						controller = NEW(NxController(controllerId));

						Event e;
						e.device = Event::deviceController;
						e.controller.type = Event::Controller::typeDeviceAdded;
						e.controller.device = controllerId;
						AddEvent(e);
					}
					else
					{
						controller->active = false;
						controller = nullptr;

						Event e;
						e.device = Event::deviceController;
						e.controller.type = Event::Controller::typeDeviceRemoved;
						e.controller.device = controllerId;
						AddEvent(e);
					}
				}

				// process input
				if(isConnected)
				{
					// buttons
					auto updateButton = [&](Event::Controller::Button button, bool buttonState)
					{
						if(!!(controller->buttons & (1 << button)) != buttonState)
						{
							Event e;
							e.device = Event::deviceController;
							e.controller.device = controllerId;
							e.controller.button = button;
							if(buttonState)
							{
								controller->buttons |= (1 << button);
								e.controller.type = Event::Controller::typeButtonDown;
							}
							else
							{
								controller->buttons &= ~(1 << button);
								e.controller.type = Event::Controller::typeButtonUp;
							}
							AddEvent(e);
						}
					};
#define B(a, b) updateButton(Event::Controller::Button::button##b, npadState.buttons.template Test<nn::hid::NpadButton::a>())
					B(A, A);
					B(B, B);
					B(X, X);
					B(Y, Y);
					B(Minus, Back);
					// B(, Guide);
					B(Plus, Start);
					B(StickL, LeftStick);
					B(StickR, RightStick);
					B(L, LeftShoulder);
					B(R, RightShoulder);
					B(Up, DPadUp);
					B(Down, DPadDown);
					B(Left, DPadLeft);
					B(Right, DPadRight);
#undef B
					// convert triggers (buttons) to axis
					auto updateTrigger = [&](Event::Controller::Axis axis, bool& buttonState, bool newButtonState)
					{
						if(buttonState != newButtonState)
						{
							buttonState = newButtonState;

							Event e;
							e.device = Event::deviceController;
							e.controller.type = Event::Controller::typeAxisMotion;
							e.controller.device = controllerId;
							e.controller.axis = axis;
							e.controller.axisValue = buttonState ? 0x7fff : 0;
							AddEvent(e);
						}
					};
					updateTrigger(Event::Controller::Axis::axisTriggerLeft, controller->leftTrigger, npadState.buttons.template Test<nn::hid::NpadButton::ZL>());
					updateTrigger(Event::Controller::Axis::axisTriggerRight, controller->rightTrigger, npadState.buttons.template Test<nn::hid::NpadButton::ZR>());

					// sticks
					auto updateStick = [&](Event::Controller::Axis axis, int& value, int32_t newValue)
					{
						if(value != newValue)
						{
							value = newValue;

							Event e;
							e.device = Event::deviceController;
							e.controller.type = Event::Controller::typeAxisMotion;
							e.controller.device = controllerId;
							e.controller.axis = axis;
							e.controller.axisValue = value;
							AddEvent(e);
						}
					};
#define S(a, b, c) updateStick(Event::Controller::Axis::axis##a, controller->b, c)
					S(LeftX, leftStickX, npadState.analogStickL.x);
					S(LeftY, leftStickY, -npadState.analogStickL.y);
					S(RightX, rightStickX, npadState.analogStickR.x);
					S(RightY, rightStickY, -npadState.analogStickR.y);
#undef S
				}
			}
		};

		// choose state style depending on controller id
		if(controllerId == nn::hid::NpadId::Handheld)
		{
			nn::hid::NpadHandheldState npadStates[nn::hid::NpadStateCountMax];
			int statesRead = nn::hid::GetNpadStates(npadStates, sizeof(npadStates) / sizeof(npadStates[0]), controllerId);
			processStates(npadStates, statesRead);
		}
		else
		{
			nn::hid::NpadFullKeyState npadStates[nn::hid::NpadStateCountMax];
			int statesRead = nn::hid::GetNpadStates(npadStates, sizeof(npadStates) / sizeof(npadStates[0]), controllerId);
			processStates(npadStates, statesRead);
		}
	}

	Manager::Update();
}

void NxManager::StartTextInput()
{
	// TODO
}

void NxManager::StopTextInput()
{
	// TODO
}

ptr<Controller> NxManager::TryGetController(uint64_t controllerId)
{
	Controllers::const_iterator i = controllers.find(controllerId);
	if(i == controllers.end()) return nullptr;
	return i->second.second;
}

END_INANITY_INPUT
