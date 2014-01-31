#ifndef ___INANITY_CONFIG_ALLOCA_HPP___
#define ___INANITY_CONFIG_ALLOCA_HPP___

#include "../config.hpp"

#if defined(___INANITY_PLATFORM_FREEBSD)
#include <stdlib.h>
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_EMSCRIPTEN)
#include <alloca.h>
#elif defined(___INANITY_PLATFORM_WINDOWS)
#include <malloc.h>
#else
#error Unknown platform
#endif

#endif
