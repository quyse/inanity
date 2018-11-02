#ifndef ___INANITY_XA_PLAYER_HPP___
#define ___INANITY_XA_PLAYER_HPP___

#include "Player3D.hpp"
#include "xaudio2.hpp"
#include <queue>
#include <atomic>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

class XaDevice;

/// Внутренний класс подсистемы XAudio2, инкапсулирующий интерфейс IXAudio2SourceVoice.
/** Также реализует интерфейс IXAudio2VoiceCallback для принятия уведомлений. */
class XaPlayer : public Player3D, public IXAudio2VoiceCallback
{
protected:
	ptr<XaDevice> device;
	IXAudio2SourceVoice* voice;

	/// Очередь из буферов на проигрывание.
	std::queue<ptr<File> > files;

	float volume, pitch;

	bool playing;

	std::atomic<size_t> onBufferEndCounter;

	XaPlayer(ptr<XaDevice> device);
	~XaPlayer();

	/// Установить или удалить voice.
	void SetVoice(IXAudio2SourceVoice* voice);

	/// Добавить буфер для проигрывания.
	void PushBuffer(ptr<File> file);

	/// Начать проигрывание.
	void StartPlaying();
	/// Остановить проигрывание.
	void StopPlaying();

	void ApplyVolume();
	void ApplyPitch();

public:
	/// Регулярная обработка.
	virtual void Tick();

	//*** Some Player's methods.
	bool IsPlaying() const override;
	void SetVolume(float volume) override;
	void SetPitch(float pitch) override;
	//** Player3D's methods.
	void SetPosition(const Math::vec3& position) override;
	void SetDirection(const Math::vec3& direction) override;
	void SetVelocity(const Math::vec3& velocity) override;

	//*** методы IXAudio2VoiceCallback
	void CALLBACK OnVoiceProcessingPassStart(UINT32 BytesRequired) override;
	void CALLBACK OnVoiceProcessingPassEnd() override;
	void CALLBACK OnStreamEnd() override;
	void CALLBACK OnBufferStart(void* pBufferContext) override;
	void CALLBACK OnBufferEnd(void* pBufferContext) override;
	void CALLBACK OnLoopEnd(void* pBufferContext) override;
	void CALLBACK OnVoiceError(void* pBufferContext, HRESULT Error) override;
};

END_INANITY_AUDIO

#endif
