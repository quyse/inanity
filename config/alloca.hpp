#ifndef ___INANITY_CONFIG_ALLOCA_HPP___
#define ___INANITY_CONFIG_ALLOCA_HPP___

#include "../config.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include <malloc.h>
#else
#include <stdlib.h>
#endif

#endif
