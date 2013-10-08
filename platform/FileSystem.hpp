#ifndef ___INANITY_PLATFORM_FILE_SYSTEM_HPP___
#define ___INANITY_PLATFORM_FILE_SYSTEM_HPP___

#include "platform.hpp"

#ifdef ___INANITY_WINDOWS
#include "Win32FileSystem.hpp"
#endif

#ifdef ___INANITY_LINUX
#include "PosixFileSystem.hpp"
#endif

BEGIN_INANITY_PLATFORM

#ifdef ___INANITY_WINDOWS

typedef Win32FileSystem FileSystem;

#endif

#ifdef ___INANITY_LINUX

typedef PosixFileSystem FileSystem;

#endif

END_INANITY_PLATFORM

#endif
