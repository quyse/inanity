#ifndef ___INANITY_AUDIO_AUDIO_HPP___
#define ___INANITY_AUDIO_AUDIO_HPP___

/* Общий файл для подсистемы звука. */

#include "../Object.hpp"

#define BEGIN_INANITY_AUDIO BEGIN_INANITY namespace Audio {
#define END_INANITY_AUDIO } END_INANITY
#ifdef INANITY_LIB
BEGIN_INANITY_AUDIO
END_INANITY_AUDIO
using namespace Inanity::Audio;
#endif

#endif
