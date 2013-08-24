#ifndef ___INANITY_AUDIO_AL_STREAMED_PLAYER_HPP___
#define ___INANITY_AUDIO_AL_STREAMED_PLAYER_HPP___

#include "AlPlayer.hpp"

BEGIN_INANITY

class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

class Source;
class AlBuffer;

/// OpenAL player with streaming.
class AlStreamedPlayer : public AlPlayer
{
private:
	/// Length of sound in one buffer, in ms.
	static const size_t bufferTime = 500;
	/// Number of buffers.
	static const int buffersCount = 2;

	ptr<Source> source;

	/// Size of one buffer in bytes.
	size_t bufferSize;

	/// Currently played stream.
	/** Non-zero only if there is some data in it. */
	ptr<InputStream> stream;
	/// Is playing in progress.
	bool playing;

	ptr<AlBuffer> buffers[buffersCount];

	/// Reads data from source, and uploads to specified buffer.
	/** \returns size of uploaded data. */
	size_t Fill(ptr<AlBuffer>& buffer);

public:
	AlStreamedPlayer(ptr<AlDevice> device, ptr<Source> source);
	~AlStreamedPlayer();

	/// Process streaming.
	void Process();

	//** Player's methods.
	void Play(int repeat = 1);
	void Pause();
	void Stop();
	bool IsPlaying() const;
};

END_INANITY_AUDIO

#endif
