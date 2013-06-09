#ifndef ___INANITY_INANITY_PLATFORM_HPP___
#define ___INANITY_INANITY_PLATFORM_HPP___

#include "platform/Desktop.hpp"
#include "platform/Game.hpp"
#include "platform/Window.hpp"
#ifdef ___INANITY_WINDOWS
#include "platform/Win32Window.hpp"
#endif
#ifdef ___INANITY_LINUX
#include "platform/X11Display.hpp"
#include "platform/X11Window.hpp"
#endif

#endif
