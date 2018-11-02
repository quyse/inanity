#ifndef ___INANITY_AUDIO_XA_BUFFERED_PLAYER_HPP___
#define ___INANITY_AUDIO_XA_BUFFERED_PLAYER_HPP___

#include "XaPlayer.hpp"

BEGIN_INANITY_AUDIO

class XaBufferedSound;

class XaBufferedPlayer : public XaPlayer
{
private:
	ptr<XaBufferedSound> sound;

public:
	XaBufferedPlayer(ptr<XaBufferedSound> sound);

	//** Player's methods.
	void Play(bool looped) override;
	void Pause() override;
	void Stop() override;
};

END_INANITY_AUDIO

#endif
