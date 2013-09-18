#ifndef ___INANITY_PROFILE_PROFILE_HPP___
#define ___INANITY_PROFILE_PROFILE_HPP___

#include "../Object.hpp"

#ifdef _DEBUG
// Enable profiling.
#define ___INANITY_PROFILING
#endif

#define BEGIN_INANITY_PROFILE BEGIN_INANITY namespace Profile {
#define END_INANITY_PROFILE } END_INANITY

#endif
