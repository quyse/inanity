#ifndef ___INANITY_XA_SOURCE_VOICE_HPP___
#define ___INANITY_XA_SOURCE_VOICE_HPP___

#include "audio.hpp"
#include "xaudio2.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_AUDIO

/// Внутренний класс подсистемы XAudio2, инкапсулирующий интерфейс IXAudio2SourceVoice.
/** Также реализует интерфейс IXAudio2VoiceCallback для принятия уведомлений. */
class XASourceVoice : public Object, private IXAudio2VoiceCallback
{
private:
	ptr<XASystem> system;
	ComPointer<IXAudio2SourceVoice> voice;

	/// Очередь из буферов на проигрывание.
	std::queue<ptr<File> file>

public:
	XASourceVoice(ptr<XASystem> system);

	/// Установить интерфейс в Voice.
	/** Не в конструкторе, потому что объект создаётся раньше интерфейса. */
	void SetVoice(ComPointer<IXAudio2SourceVoice> voice);

	/// Добавить буфер для проигрывания.
	void PushBuffer(ptr<File> data);

private:
	//*** методы IXAudio2VoiceCallback
	void CALLBACK OnVoiceProcessingPassStart(UINT32 BytesRequired);
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