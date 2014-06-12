#ifndef ___INANITY_PLATFORM_NPAPI_HPP___
#define ___INANITY_PLATFORM_NPAPI_HPP___

#include "platform.hpp"
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "windows.hpp"
#endif

// for some reason, NPAPI doesn't define XP_UNIX in any case
#if defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
#define XP_UNIX
#endif

#include "../deps/npapi/npfunctions.h"

#endif
