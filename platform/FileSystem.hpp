#ifndef ___INANITY_PLATFORM_FILE_SYSTEM_HPP___
#define ___INANITY_PLATFORM_FILE_SYSTEM_HPP___

#include "platform.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)

#include "Win32FileSystem.hpp"

BEGIN_INANITY_PLATFORM
typedef Win32FileSystem FileSystem;
END_INANITY_PLATFORM

#elif defined(___INANITY_PLATFORM_SWITCH)

#include "NxFileSystem.hpp"

BEGIN_INANITY_PLATFORM
typedef NxFileSystem FileSystem;
END_INANITY_PLATFORM

#elif defined(___INANITY_PLATFORM_POSIX)

#include "PosixFileSystem.hpp"

BEGIN_INANITY_PLATFORM
typedef PosixFileSystem FileSystem;
END_INANITY_PLATFORM

#else
#error Unknown platform
#endif

#endif
