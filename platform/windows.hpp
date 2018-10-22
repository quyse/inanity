#ifndef ___INANITY_PLATFORM_WINDOWS_HPP___
#define ___INANITY_PLATFORM_WINDOWS_HPP___

#ifndef ___INANITY_PLATFORM_WINDOWS
#error This is not windows!
#endif

#define STRICT
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define UNICODE
#define _UNICODE
#if defined(_XBOX_ONE)
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#else
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif
#include <windows.h>

#endif
