#ifndef ___INANITY_AUDIO_XA_STREAMED_PLAYER_HPP___
#define ___INANITY_AUDIO_XA_STREAMED_PLAYER_HPP___

#include "XaPlayer.hpp"

BEGIN_INANITY_AUDIO

class XaStreamedSound;

class XaStreamedPlayer : public XaPlayer
{
private:
	ptr<XaStreamedSound> sound;

public:
	XaStreamedPlayer(ptr<XaStreamedSound> sound);

	//** Player's methods.
	void Play(bool looped) override;
	void Pause() override;
	void Stop() override;
};

END_INANITY_AUDIO

#endif
