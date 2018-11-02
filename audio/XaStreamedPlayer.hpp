#ifndef ___INANITY_AUDIO_XA_STREAMED_PLAYER_HPP___
#define ___INANITY_AUDIO_XA_STREAMED_PLAYER_HPP___

#include "XaPlayer.hpp"

BEGIN_INANITY

class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

class XaStreamedSound;

class XaStreamedPlayer : public XaPlayer
{
private:
	/// Length of sound in one buffer, in ms.
	static const size_t bufferTime = 500;
	/// Number of buffers.
	static const int buffersCount = 3;

	ptr<XaStreamedSound> sound;
	ptr<InputStream> stream;

	/// Size of one buffer in bytes.
	size_t bufferSize;

	bool looping;

	bool FillBuffer();
	void FillBuffers();

public:
	XaStreamedPlayer(ptr<XaStreamedSound> sound);

	void Tick() override;

	//** Player's methods.
	void Play(bool looped) override;
	void Pause() override;
	void Stop() override;
};

END_INANITY_AUDIO

#endif
