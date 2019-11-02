#ifndef ___INANITY_AUDIO_NX_SYSTEM_HPP___
#define ___INANITY_AUDIO_NX_SYSTEM_HPP___

#include "System.hpp"
#include <nn/mem.h>
#include <nn/audio.h>
#include <unordered_set>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

class NxPlayer;
struct Format;

class NxSystem : public System
{
private:
	nn::mem::StandardAllocator workPoolAllocator;
	nn::mem::StandardAllocator wavePoolAllocator;
	nn::audio::AudioRendererHandle renderer;
	nn::os::SystemEvent rendererEvent;
	nn::audio::AudioRendererConfig config;
	nn::audio::FinalMixType finalMix;
	nn::audio::DeviceSinkType deviceSink;
	nn::audio::MemoryPoolType wavePool;

	typedef std::unordered_set<NxPlayer*> Players;
	Players players;

	class BufferFile;

public:
	NxSystem(void* workPoolPtr, size_t workPoolSize, void* wavePoolPtr, size_t wavePoolSize, int maxVoiceCount);
	~NxSystem();

	nn::audio::AudioRendererConfig& GetConfig();

	ptr<File> CreateBufferFile(size_t size);
	ptr<File> CreateBufferFile(ptr<File> file);

	void CreateVoice(nn::audio::VoiceType& voice, const Format& format);
	void DeleteVoice(nn::audio::VoiceType& voice);
	void SetVoicePanning(nn::audio::VoiceType& voice, int channelsCount, float pan);

	void RegisterPlayer(NxPlayer* player);
	void UnregisterPlayer(NxPlayer* player);

	// System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
