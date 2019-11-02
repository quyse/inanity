#ifndef ___INANITY_AUDIO_NX_PLAYER_HPP___
#define ___INANITY_AUDIO_NX_PLAYER_HPP___

#include "Player.hpp"
#include <nn/audio.h>
#include <queue>

BEGIN_INANITY_AUDIO

class NxSystem;
struct Format;

class NxPlayer : public Player
{
private:
	ptr<NxSystem> system;
	nn::audio::VoiceType voice;
	int channelsCount;

	/// Queue of free buffers.
	std::queue<const nn::audio::WaveBuffer*> bufferQueue;
	/// Number of buffers appended to the voice.
	size_t appendedBuffersCount;
	/// Voice is in "play" state?
	bool playing;
	/// Playing was started in looped state?
	bool looping;
	/// Appending new blocks is stopped?
	bool stopped;

protected:
	void QueueWaveBuffer(const nn::audio::WaveBuffer* buffer);
	void AppendWaveBuffer(const nn::audio::WaveBuffer* buffer);

	/// Append wave buffer with more data if possible.
	/** Return false if there's no more data. */
	virtual bool RequestWaveBuffer(const nn::audio::WaveBuffer* buffer) = 0;
	/// Reset after end of data.
	virtual void Reset() = 0;

public:
	NxPlayer(ptr<NxSystem> system, const Format& format);
	~NxPlayer();

	/// Tick method.
	void Tick();

	// Player's methods.
	void Play(bool looped) override;
	void Pause() override;
	void Stop() override;
	bool IsPlaying() const override;
	void SetVolume(float volume) override;
	void SetPanning(float pan) override;
	void SetPitch(float pitch) override;
};

END_INANITY_AUDIO

#endif
