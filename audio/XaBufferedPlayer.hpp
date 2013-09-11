#ifndef ___INANITY_AUDIO_XA_BUFFERED_PLAYER_HPP___
#define ___INANITY_AUDIO_XA_BUFFERED_PLAYER_HPP___

#include "XaPlayer.hpp"
#include "xaudio2.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

/// XAudio2 buffered player.
class XaBufferedPlayer : public XaPlayer
{
private:
	ptr<File> file;

public:
	XaBufferedPlayer(ptr<XaDevice> device, const Format& format, ptr<File> file);

	/// Player's methods.
	void Play(bool looped = false);
	void Pause();
	void Stop();
	bool IsPlaying() const;
};

END_INANITY_AUDIO

#endif
