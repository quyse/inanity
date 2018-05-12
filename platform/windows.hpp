#ifndef ___INANITY_PLATFORM_WINDOWS_HPP___
#define ___INANITY_PLATFORM_WINDOWS_HPP___

#ifndef ___INANITY_PLATFORM_WINDOWS
#error This is not windows!
#endif

#define STRICT
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <windows.h>

#endif
