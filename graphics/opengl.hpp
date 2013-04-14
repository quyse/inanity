#ifndef ___INANITY_GRAPHICS_OPENGL_HPP___
#define ___INANITY_GRAPHICS_OPENGL_HPP___

#include "../Object.hpp"

#ifdef ___INANITY_WINDOWS
#define GLEW_STATIC
#include "../deps/glew/include/GL/glew.h"
#include "../windows.hpp"
#include "../deps/glew/include/GL/wglew.h"
#endif

#ifdef ___INANITY_LINUX
#include <GL/glew.h>
//#include "../deps/glew/include/GL/glxew.h"
#include <GL/glxew.h>
#endif

#include <GL/gl.h>

#endif
