#include "AlSystem.hpp"
#include "AlDevice.hpp"
#include "Format.hpp"
#include "AlStreamedPlayer.hpp"
#include "../Thread.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_AUDIO

void AlSystem::CheckErrors(const char* primaryExceptionString)
{
	ALenum error = alGetError();
	// in case of error
	if(error != AL_NO_ERROR)
	{
		// get error string
		const char* errorString;

#define DEFINE_AL_ERROR(code) \
	case code: \
		errorString = #code; \
		break

		switch(error)
		{
			DEFINE_AL_ERROR(AL_INVALID_NAME);
			DEFINE_AL_ERROR(AL_INVALID_ENUM);
			DEFINE_AL_ERROR(AL_INVALID_VALUE);
			DEFINE_AL_ERROR(AL_INVALID_OPERATION);
			DEFINE_AL_ERROR(AL_OUT_OF_MEMORY);
		default:
			errorString = "Unknown OpenAL error";
			break;
		}

#undef DEFINE_GL_ERROR

		if(primaryExceptionString)
			THROW_SECONDARY(primaryExceptionString, NEW(Exception(errorString)));
		else
			THROW(errorString);
	}
}

ALenum AlSystem::ConvertFormat(const Format& format)
{
	switch(format.channelsCount)
	{
	case 1:
		switch(format.bitsPerSample)
		{
		case 8: return AL_FORMAT_MONO8;
		case 16: return AL_FORMAT_MONO16;
		}
		break;
	case 2:
		switch(format.bitsPerSample)
		{
		case 8: return AL_FORMAT_STEREO8;
		case 16: return AL_FORMAT_STEREO16;
		}
		break;
	}

	THROW("Unsupported OpenAL buffer format");
}

void AlSystem::RegisterStreamedPlayer(AlStreamedPlayer* player)
{
	streamedPlayers.push_back(player);
}

void AlSystem::UnregisterStreamedPlayer(AlStreamedPlayer* player)
{
	std::vector<AlStreamedPlayer*>::iterator i =
		std::find(streamedPlayers.begin(), streamedPlayers.end(), player);
	if(i != streamedPlayers.end())
		streamedPlayers.erase(i);
}

ptr<Device> AlSystem::CreateDefaultDevice()
{
	BEGIN_TRY();

	::ALCdevice* device = alcOpenDevice(0);
	if(!device)
		THROW("Can't create device");

	return NEW(AlDevice(this, device));

	END_TRY("Can't create default OpenAL device");
}

void AlSystem::Tick()
{
	for(size_t i = 0; i < streamedPlayers.size(); ++i)
		streamedPlayers[i]->Process();
}

END_INANITY_AUDIO
