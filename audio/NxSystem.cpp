#include "NxSystem.hpp"
#include "NxDevice.hpp"
#include "NxPlayer.hpp"
#include "Format.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

class NxSystem::BufferFile : public File
{
private:
	ptr<NxSystem> system;
	void* data;
	size_t size;

public:
	BufferFile(ptr<NxSystem> system, ptr<File> file)
	: system(system)
	{
		size = file->GetSize();
		data = system->wavePoolAllocator.Allocate(size, nn::audio::BufferAlignSize);
		if(!data)
			THROW("Can't allocate buffer from wave pool");
		memcpy(data, (const void*)file->GetData(), size);
	}

	~BufferFile()
	{
		system->wavePoolAllocator.Free(data);
	}

	void* GetData() const override
	{
		return data;
	}

	size_t GetSize() const override
	{
		return size;
	}
};

NxSystem::NxSystem(void* workPoolPtr, size_t workPoolSize, void* wavePoolPtr, size_t wavePoolSize, int maxVoiceCount)
: workPoolAllocator(workPoolPtr, workPoolSize)
, wavePoolAllocator(wavePoolPtr, wavePoolSize)
{
	BEGIN_TRY();

	// init audio parameter
	nn::audio::AudioRendererParameter parameter;
	nn::audio::InitializeAudioRendererParameter(&parameter);
	parameter.voiceCount = maxVoiceCount;
	parameter.mixBufferCount = 2;

	// check validity of parameter
	if(!nn::audio::IsValidAudioRendererParameter(parameter))
		THROW("Audio renderer parameter is invalid");

	// init renderer
	size_t workBufferSize = nn::audio::GetAudioRendererWorkBufferSize(parameter);
	void* workBuffer = workPoolAllocator.Allocate(workBufferSize, nn::os::MemoryPageSize);
	NN_ABORT_UNLESS_NOT_NULL(workBuffer);
	if(!nn::audio::OpenAudioRenderer(&renderer, &rendererEvent, parameter, workBuffer, workBufferSize).IsSuccess())
		THROW("Can't open audio renderer");

	// init config
	size_t configBufferSize = nn::audio::GetAudioRendererConfigWorkBufferSize(parameter);
	void* configBuffer = workPoolAllocator.Allocate(configBufferSize, nn::os::MemoryPageSize);
	NN_ABORT_UNLESS_NOT_NULL(configBuffer);
	nn::audio::InitializeAudioRendererConfig(&config, parameter, configBuffer, configBufferSize);

	// acquire final mix
	if(!nn::audio::AcquireFinalMix(&config, &finalMix, 2))
		THROW("Can't acquire final mix");

	// init sink
	int8_t channelMapping[2];
	channelMapping[nn::audio::ChannelMapping_FrontLeft] = 0;
	channelMapping[nn::audio::ChannelMapping_FrontRight] = 1;
	if(!nn::audio::AddDeviceSink(&config, &deviceSink, &finalMix, channelMapping, sizeof(channelMapping) / sizeof(channelMapping[0]), "MainAudioOut").IsSuccess())
		THROW("Can't add device sink");

	// start rendering
	if(!nn::audio::RequestUpdateAudioRenderer(renderer, &config).IsSuccess())
		THROW("Can't update audio renderer");
	if(!nn::audio::StartAudioRenderer(renderer).IsSuccess())
		THROW("Can't start audio renderer");

	// prepare wave memory pool
	if(!nn::audio::AcquireMemoryPool(&config, &wavePool, wavePoolPtr, wavePoolSize))
		THROW("Can't acquire memory pool");
	if(!nn::audio::RequestAttachMemoryPool(&wavePool))
		THROW("Can't attach memory pool");

	END_TRY("Can't create audio NxSystem");
}

NxSystem::~NxSystem()
{
	nn::audio::StopAudioRenderer(renderer);
	nn::audio::ReleaseFinalMix(&config, &finalMix);
	nn::audio::CloseAudioRenderer(renderer);
	nn::os::DestroySystemEvent(rendererEvent.GetBase());
}

nn::audio::AudioRendererConfig& NxSystem::GetConfig()
{
	return config;
}

ptr<File> NxSystem::CreateBufferFile(ptr<File> file)
{
	return NEW(BufferFile(this, file));
}

void NxSystem::CreateVoice(nn::audio::VoiceType& voice, const Format& format)
{
	// get sample format
	nn::audio::SampleFormat sampleFormat;
	switch(format.bitsPerSample)
	{
	// 8-bit is not supported actually
	// case 8:
	// 	sampleFormat = nn::audio::SampleFormat_PcmInt8;
	// 	break;
	case 16:
		sampleFormat = nn::audio::SampleFormat_PcmInt16;
		break;
	default:
		THROW("Unknown sample format");
	}

	// acquire slot
	if(!nn::audio::AcquireVoiceSlot(&config, &voice, format.samplesPerSecond, format.channelsCount, sampleFormat, nn::audio::VoiceType::PriorityHighest, nullptr, 0))
		THROW("Can't acquire voice slot");

	// attach to final mix
	nn::audio::SetVoiceDestination(&config, &voice, &finalMix);
}

void NxSystem::DeleteVoice(nn::audio::VoiceType& voice)
{
	nn::audio::ReleaseVoiceSlot(&config, &voice);
}

void NxSystem::SetVoicePanning(nn::audio::VoiceType& voice, float pan)
{
	// left
	nn::audio::SetVoiceMixVolume(&voice, &finalMix, 0.5f - pan * 0.5f, 0, 0);
	// right
	nn::audio::SetVoiceMixVolume(&voice, &finalMix, 0.5f + pan * 0.5f, 0, 1);
}

void NxSystem::RegisterPlayer(NxPlayer* player)
{
	players.insert(player);
}

void NxSystem::UnregisterPlayer(NxPlayer* player)
{
	players.erase(player);
}

ptr<Device> NxSystem::CreateDefaultDevice()
{
	return NEW(NxDevice(this));
}

void NxSystem::Tick()
{
	for(Players::const_iterator i = players.begin(); i != players.end(); ++i)
		(*i)->Tick();

	if(!nn::audio::RequestUpdateAudioRenderer(renderer, &config).IsSuccess())
		THROW("Can't update audio renderer");
}

END_INANITY_AUDIO
