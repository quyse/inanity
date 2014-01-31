#ifndef ___INANITY_CONFIG_ALLOCA_HPP___
#define ___INANITY_CONFIG_ALLOCA_HPP___

#include "../config.hpp"

#if __clang__
#include <stdlib.h>
#elif __GNUC__
#include <alloca.h>
#else
#include <malloc.h>
#endif

#endif
