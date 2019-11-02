#include "NxDevice.hpp"
#include "NxSystem.hpp"
#include "NxBufferedSound.hpp"
#include "NxStreamedSound.hpp"
#include "Source.hpp"
#include "../File.hpp"

BEGIN_INANITY_AUDIO

NxDevice::NxDevice(ptr<NxSystem> system)
: system(system) {}

ptr<NxSystem> NxDevice::GetSystem() const
{
	return system;
}

ptr<Sound> NxDevice::CreateBufferedSound(ptr<Source> source)
{
	return NEW(NxBufferedSound(system, system->CreateBufferFile(source->GetData()), source->GetFormat()));
}

ptr<Sound> NxDevice::CreateStreamedSound(ptr<Source> source)
{
	return NEW(NxStreamedSound(system, source));
}

void NxDevice::SetListenerPosition(const Math::vec3& position)
{
}

void NxDevice::SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up)
{
}

void NxDevice::SetListenerVelocity(const Math::vec3& velocity)
{
}

END_INANITY_AUDIO
