#include "NxSystem.hpp"
#include "NxDevice.hpp"

BEGIN_INANITY_AUDIO

ptr<Device> NxSystem::CreateDefaultDevice()
{
	return NEW(NxDevice()); // TODO
}

void NxSystem::Tick()
{
	// TODO
}

END_INANITY_AUDIO
