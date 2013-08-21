#include "AlSystem.hpp"
#include "AlDevice.hpp"
#include "../Exception.hpp"

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
}

END_INANITY_AUDIO
