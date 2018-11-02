#include "XaSystem.hpp"
#include "XaDevice.hpp"
#include "XaPlayer.hpp"
#include "Format.hpp"
#include "../CriticalCode.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaSystem::XaSystem()
{
	BEGIN_TRY();

	if(FAILED(XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		THROW("Can't create XAudio2 object");

	END_TRY("Can't create XAudio2 system");
}

IXAudio2SourceVoice* XaSystem::AllocateSourceVoice(const Format& format, IXAudio2VoiceCallback* callback)
{
	IXAudio2SourceVoice* voiceInterface;
	if(FAILED(xAudio2->CreateSourceVoice(&voiceInterface, &ConvertFormat(format), 0, XAUDIO2_DEFAULT_FREQ_RATIO, callback, NULL, NULL)))
		THROW("Can't create source voice");

	return voiceInterface;
}

void XaSystem::RegisterPlayer(XaPlayer* player)
{
	CriticalCode cc(csPlayers);

	players.insert(player);
}

void XaSystem::UnregisterPlayer(XaPlayer* player)
{
	CriticalCode cc(csPlayers);

	players.erase(player);
}

ptr<Device> XaSystem::CreateDefaultDevice()
{
	BEGIN_TRY();

	// создать объект устройства
	IXAudio2MasteringVoice* voice;
	if(FAILED(xAudio2->CreateMasteringVoice(&voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL)))
		THROW("Can't create mastering voice");

	// всё
	return NEW(XaDevice(this, voice));

	END_TRY("Can't create default XAudio2 device");
}

void XaSystem::Tick()
{
	{
		CriticalCode cc(csPlayers);

		tempPlayers.assign(players.begin(), players.end());
	}

	for(size_t i = 0; i < tempPlayers.size(); ++i)
	{
		tempPlayers[i]->Tick();
		tempPlayers[i] = nullptr;
	}

	tempPlayers.clear();
}

WAVEFORMATEX XaSystem::ConvertFormat(const Format& format)
{
	WAVEFORMATEX r;
	r.wFormatTag = WAVE_FORMAT_PCM;
	r.nChannels = format.channelsCount;
	r.nSamplesPerSec = format.samplesPerSecond;
	r.wBitsPerSample = format.bitsPerSample;
	r.nBlockAlign = r.nChannels * r.wBitsPerSample / 8;
	r.nAvgBytesPerSec = r.nBlockAlign * r.nSamplesPerSec;
	r.cbSize = 0;

	return r;
}

END_INANITY_AUDIO
