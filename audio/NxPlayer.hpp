#ifndef ___INANITY_AUDIO_NX_PLAYER_HPP___
#define ___INANITY_AUDIO_NX_PLAYER_HPP___

#include "Player.hpp"
#include <nn/audio.h>

BEGIN_INANITY_AUDIO

class NxSystem;
struct Format;

class NxPlayer : public Player
{
protected:
	ptr<NxSystem> system;
	nn::audio::VoiceType voice;
	int channelsCount;

public:
	NxPlayer(ptr<NxSystem> system, const Format& format);
	~NxPlayer();

	/// Tick method.
	virtual void Tick() = 0;

	// Player's methods.
	bool IsPlaying() const override;
	void SetVolume(float volume) override;
	void SetPanning(float pan) override;
	void SetPitch(float pitch) override;
};

END_INANITY_AUDIO

#endif
