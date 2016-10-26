#ifndef ___INANITY_AUDIO_AL_HPP___
#define ___INANITY_AUDIO_AL_HPP___

#if defined(___INANITY_PLATFORM_MACOS)
#include </System/Library/Frameworks/OpenAL.framework/Headers/al.h>
#include </System/Library/Frameworks/OpenAL.framework/Headers/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#endif
