#include "XaSystem.hpp"
#include "XaDevice.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaSystem::XaSystem()
{
	BEGIN_TRY();

	HRESULT hr = XAudio2Create(&xAudio2,
#ifdef _DEBUG
		XAUDIO2_DEBUG_ENGINE
#else
		0
#endif
		, XAUDIO2_DEFAULT_PROCESSOR);
	if(FAILED(hr))
		THROW("Can't create XAudio2 object");

	if(FAILED(xAudio2->StartEngine()))
		THROW("Can't start engine");

	END_TRY("Can't create XAudio2 system");
}

XaSystem::~XaSystem()
{
	xAudio2->StopEngine();
}

IXAudio2* XaSystem::GetInterface() const
{
	return xAudio2;
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

ptr<Device> XaSystem::CreateDefaultDevice()
{
	try
	{
		// получить количество устройств
		UINT devicesCount;
		if(FAILED(xAudio2->GetDeviceCount(&devicesCount)))
			THROW("Can't get devices count");

		// если устройств вообще нет
		if(!devicesCount)
			THROW("No audio devices found");

		// перебрать все устройства, и выбрать самое подходящее
		int preferredDevice = -1;
		for(UINT i = 0; i < devicesCount; ++i)
		{
			XAUDIO2_DEVICE_DETAILS details;
			if(FAILED(xAudio2->GetDeviceDetails(i, &details)))
				THROW("Can't get device details");
			// если устройство является устройством по умолчанию
			if(details.Role & DefaultGameDevice)
				// выбрать его
				preferredDevice = i;
		}

		// если устройство не выбрано
		if(preferredDevice < 0)
			// выбрать первое
			preferredDevice = 0;

		// создать объект устройства
		IXAudio2MasteringVoice* voice;
		if(FAILED(xAudio2->CreateMasteringVoice(&voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, preferredDevice, NULL)))
			THROW("Can't create mastering voice");

		// всё
		return NEW(XaDevice(this, voice));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create default XAudio2 device", exception);
	}
}

void XaSystem::Tick()
{
}

END_INANITY_AUDIO
