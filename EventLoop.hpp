#ifndef ___INANITY_EVENT_LOOP_HPP___
#define ___INANITY_EVENT_LOOP_HPP___

#include "Object.hpp"

#ifdef ___INANITY_WINDOWS

#include "WindowsEventLoop.hpp"
BEGIN_INANITY
typedef WindowsEventLoop EventLoop;
END_INANITY

#endif

#ifdef ___INANITY_LINUX

#include "LibevEventLoop.hpp"
BEGIN_INANITY
typedef LibevEventLoop EventLoop;
END_INANITY

#endif

#endif
