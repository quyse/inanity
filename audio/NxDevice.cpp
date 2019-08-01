#include "NxDevice.hpp"
#include "Player3D.hpp"
#include "Sound.hpp"

BEGIN_INANITY_AUDIO

// FIXME
class NxDummyPlayer : public Player3D
{
public:
	// Player's methods.
	void Play(bool looped = false) override {}
	void Pause() override {}
	void Stop() override {}
	bool IsPlaying() const override { return false; }
	void SetVolume(float volume) override {}
	void SetPanning(float pan) override {}
	void SetPitch(float pitch) override {}
	// Player3D's methods.
	void SetPosition(const Math::vec3& position) override {}
	void SetDirection(const Math::vec3& direction) override {}
	void SetVelocity(const Math::vec3& velocity) override {}
};
class NxDummySound : public Sound
{
public:
	ptr<Player> CreatePlayer() override
	{
		return NEW(NxDummyPlayer());
	}

	ptr<Player3D> CreatePlayer3D() override
	{
		return NEW(NxDummyPlayer());
	}
};

NxDevice::NxDevice()
{
}

ptr<Sound> NxDevice::CreateBufferedSound(ptr<Source> source)
{
	return NEW(NxDummySound());
}

ptr<Sound> NxDevice::CreateStreamedSound(ptr<Source> source)
{
	return NEW(NxDummySound());
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
