#ifndef ___INANITY_AUDIO_XA_SYSTEM_HPP___
#define ___INANITY_AUDIO_XA_SYSTEM_HPP___

#include "System.hpp"
#include "Format.hpp"
#include "xaudio2.hpp"
#include "../CriticalSection.hpp"
#include "../ComPointer.hpp"
#include <unordered_set>

BEGIN_INANITY_AUDIO

class Device;
struct Format;
class XaPlayer;

/// Класс звуковой подсистемы для XAudio.
class XaSystem : public System
{
private:
	ComPointer<IXAudio2> xAudio2;
	std::unordered_set<XaPlayer*> players;
	CriticalSection csPlayers;
	std::vector<ptr<XaPlayer> > tempPlayers;

public:
	XaSystem();

	/// Получить voice нужного формата.
	IXAudio2SourceVoice* AllocateSourceVoice(const Format& format, IXAudio2VoiceCallback* callback);

	/// Зарегистрировать плеер, чтобы вызывать у него тик. Потоковобезопасно.
	void RegisterPlayer(XaPlayer* player);
	/// Разрегистрировать плеер. Потоковобезопасно.
	void UnregisterPlayer(XaPlayer* player);

	static WAVEFORMATEX ConvertFormat(const Format& format);

	//** System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
