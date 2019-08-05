#include "NxManager.hpp"
#include "Frame.hpp"
#include "../platform/NxWindow.hpp"
#include <nn/hid.h>

BEGIN_INANITY_INPUT

NxManager::NxController::NxController(uint64_t controllerId)
: Controller(controllerId) {}

bool NxManager::NxController::IsActive() const
{
	return active;
}

void NxManager::NxController::RunHapticLeftRight(float left, float right)
{
	// TODO
}

void NxManager::NxController::StopHaptic()
{
	// TODO
}

NxManager::NxManager()
{
	// initialize controllers
	nn::hid::InitializeNpad();
	nn::hid::SetSupportedNpadStyleSet(nn::hid::NpadStyleFullKey::Mask | nn::hid::NpadStyleHandheld::Mask);

	// for now we support just 4 controllers at most
	const nn::hid::NpadIdType npadIds[] = {
		nn::hid::NpadId::No1,
		nn::hid::NpadId::No2,
		nn::hid::NpadId::No3,
		nn::hid::NpadId::No4
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

		nn::hid::NpadFullKeyState npadStates[nn::hid::NpadStateCountMax];
		int statesRead = nn::hid::GetNpadStates(npadStates, sizeof(npadStates) / sizeof(npadStates[0]), i->first);

		for(int j = statesRead - 1; j >= 0; --j)
		{
			const nn::hid::NpadFullKeyState& npadState = npadStates[j];

			// skip already processed states
			if(npadState.samplingNumber <= lastSamplingNumber) continue;
			lastSamplingNumber = npadState.samplingNumber;

			// process connectivity
			bool isConnected = npadState.attributes.Test<nn::hid::NpadAttribute::IsConnected>();
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
#define B(a, b) updateButton(Event::Controller::Button::button##b, npadState.buttons.Test<nn::hid::NpadButton::a>())
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
			}
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
