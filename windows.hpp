#ifndef ___INANITY_WINDOWS_HPP___
#define ___INANITY_WINDOWS_HPP___

#ifndef ___INANITY_WINDOWS
#error This is not windows!
#endif

#define STRICT
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define UNICODE
#define _UNICODE
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif
