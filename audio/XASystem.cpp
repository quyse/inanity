#include "XASystem.hpp"
#include "XADevice.hpp"
#include "../Exception.hpp"

XASystem::XASystem()
{
	try
	{
		IXAudio2* xAudio2Interface;
		if(FAILED(XAudio2Create(&xAudio2Interface,
#ifdef _DEBUG
			XAUDIO2_DEBUG_ENGINE
#else
			0
#endif
			, XAUDIO_DEFAULT_PROCESSOR)))
			THROW_PRIMARY_EXCEPTION("Can't create XAudio2 object");
		xAudio2 = xAudio2Interface;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create XAudio2 system", exception);
	}
}

ptr<Device> XASystem::CreateDefaultDevice()
{
	try
	{
		// получить количество устройств
		UINT devicesCount;
		if(FAILED(xAudio2->GetDeviceCount(&devicesCount)))
			THROW_PRIMARY_EXCEPTION("Can't get devices count");

		// если устройств вообще нет
		if(!devicesCount)
			THROW_PRIMARY_EXCEPTION("No audio devices found");

		// перебрать все устройства, и выбрать самое подходящее
		int preferredDevice = -1;
		for(UINT i = 0; i < devicesCount; ++i)
		{
			XAUDIO2_DEVICE_DETAILS details;
			if(FAILED(xAudio2->GetDeviceDetails(i, &details)))
				THROW_PRIMARY_EXCEPTION("Can't get device details");
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
		IXAudio2MasteringVoice* voiceInterface;
		if(FAILED(xAudio2->CreateMasteringVoice(&voiceInterface, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, preferredDevice, NULL)))
			THROW_PRIMARY_EXCEPTION("Can't create mastering voice");
		ComPointer<IXAudio2MasteringVoice> voice = voiceInterface;

		// всё
		return NEW(XADevice(voice));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create default XAudio2 device", exception);
	}
}

WAVEFORMATEX XASystem::ConvertFormat(const Format& format)
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

ptr<Sound> XASystem::CreateSound(ptr<Source> source)
{
}
