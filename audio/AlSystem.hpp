#ifndef ___INANITY_AUDIO_AL_SYSTEM_HPP___
#define ___INANITY_AUDIO_AL_SYSTEM_HPP___

#include "System.hpp"
#include "al.hpp"
#include <vector>

BEGIN_INANITY_AUDIO

class AlStreamedPlayer;
struct Format;

/// Class of OpenAL sound system.
class AlSystem : public System
{
private:
	/// List of streamed players.
	std::vector<AlStreamedPlayer*> streamedPlayers;

public:
	static void CheckErrors(const char* primaryExceptionString = 0);
	static ALenum ConvertFormat(const Format& format);

	void RegisterStreamedPlayer(AlStreamedPlayer* player);
	void UnregisterStreamedPlayer(AlStreamedPlayer* player);

	//** System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
