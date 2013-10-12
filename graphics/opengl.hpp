#ifndef ___INANITY_GRAPHICS_OPENGL_HPP___
#define ___INANITY_GRAPHICS_OPENGL_HPP___

#include "../Object.hpp"

#define GLEW_STATIC

#include "../deps/glew/include/GL/glew.h"

#if defined(___INANITY_PLATFORM_WINDOWS)

#include "../platform/windows.hpp"
#include "../deps/glew/include/GL/wglew.h"

#elif defined(___INANITY_PLATFORM_LINUX)

#include "../deps/glew/include/GL/glxew.h"

#else

#error Unknown platform

#endif

#include <GL/gl.h>

#endif
