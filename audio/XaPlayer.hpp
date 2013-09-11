#ifndef ___INANITY_XA_PLAYER_HPP___
#define ___INANITY_XA_PLAYER_HPP___

#include "Player3D.hpp"
#include "Format.hpp"
#include "../ComPointer.hpp"
#include "xaudio2.hpp"

BEGIN_INANITY_AUDIO

class XaSourceVoice;
class XaDevice;

/// Internal class of XAudio2 subsystem. Incapsulates IXAudio2SourceVoice.
/** Implements IXAudio2VoiceCallback to receive notifications. */
class XaPlayer : public Player3D
{
protected:
	ptr<XaSourceVoice> voice;

public:
	XaPlayer(ptr<XaDevice> device, const Format& format);
	~XaPlayer();

	//*** Player3D's methods.
	void SetPosition(const Math::vec3& position);
	void SetDirection(const Math::vec3& direction);
	void SetVelocity(const Math::vec3& velocity);
};

END_INANITY_AUDIO

#endif