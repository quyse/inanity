#ifndef ___INANITY_AUDIO_PLAYER_HPP___
#define ___INANITY_AUDIO_PLAYER_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// Abstract player class.
/** Holds a reference to a sound.
When player has lost the last reference, it remains as is
(if sound was playing, it continues, if it wasn't, it doesn't). */
class Player : public Object
{
public:
	/// Play a sound.
	virtual void Play(bool looped = false) = 0;

	/// Pause playing.
	virtual void Pause() = 0;

	/// Stop playing.
	virtual void Stop() = 0;

	/// Get if sound is playing.
	virtual bool IsPlaying() const = 0;

	/// Set volume.
	virtual void SetVolume(float volume) = 0;
	/// Set pitch.
	virtual void SetPitch(float pitch) = 0;
};

END_INANITY_AUDIO

#endif
