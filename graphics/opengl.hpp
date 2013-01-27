#ifndef ___INANITY_GRAPHICS_OPENGL_HPP___
#define ___INANITY_GRAPHICS_OPENGL_HPP___

#include "../Object.hpp"
#define GLEW_STATIC
#include "../deps/glew/include/GL/glew.h"
#ifdef ___INANITY_WINDOWS
#include "../windows.hpp"
#include "../deps/glew/include/GL/wglew.h"
#endif
#ifdef ___INANITY_LINUX
#include "../deps/glew/include/GL/glxew.h"
#endif
#include <GL/gl.h>

#endif
