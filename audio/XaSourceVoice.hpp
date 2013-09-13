#ifndef ___INANITY_AUDIO_XA_SOURCE_VOICE_HPP___
#define ___INANITY_AUDIO_XA_SOURCE_VOICE_HPP___

#include "audio.hpp"
#include "Format.hpp"
#include "xaudio2.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

class XaDevice;
class XaPlayer;

/// Class incapsulates XAudio2 source voice object.
class XaSourceVoice : public Object, private IXAudio2VoiceCallback
{
private:
	ptr<XaDevice> device;
	Format format;
	IXAudio2SourceVoice* voice;
	/// Player to receive notifications.
	XaPlayer* player;
	/// Number of buffers queued.
	int buffersCount;
	/// Is voice playing.
	bool playing;

public:
	XaSourceVoice(ptr<XaDevice> device, const Format& format);
	~XaSourceVoice();

	ptr<XaDevice> GetDevice() const;
	const Format& GetFormat() const;
	IXAudio2SourceVoice* GetVoice() const;

	/// Give voice to device.
	void Release();
	/// Grab voice from device.
	void Grab(ptr<XaDevice> device);

	/// Set player to send notifications.
	void SetPlayer(XaPlayer* player);

	/// Send a buffer to play.
	void Push(ptr<File> file, bool last);

	void Play();
	void Pause();
	void Stop();
	bool IsPlaying() const;

private:
	//*** IXAudio2VoiceCallback methods.
	void CALLBACK OnVoiceProcessingPassStart(UINT32 BytesRequired);
	void CALLBACK OnVoiceProcessingPassEnd();
	void CALLBACK OnStreamEnd();
	void CALLBACK OnBufferStart(void* pBufferContext);
	void CALLBACK OnBufferEnd(void* pBufferContext);
	void CALLBACK OnLoopEnd(void* pBufferContext);
	void CALLBACK OnVoiceError(void* pBufferContext, HRESULT Error);
};

END_INANITY_AUDIO

#endif
