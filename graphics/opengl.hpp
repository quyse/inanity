#ifndef ___INANITY_GRAPHICS_OPENGL_HPP___
#define ___INANITY_GRAPHICS_OPENGL_HPP___

#include "../Object.hpp"

#if !defined(GLEW_STATIC)
#define GLEW_STATIC
#endif
#if !defined(GLEW_NO_GLU)
#define GLEW_NO_GLU
#endif

#if defined(___INANITY_PLATFORM_WINDOWS)

#include "../deps/glew/include/GL/glew.h"
#include "../platform/windows.hpp"
#include "../deps/glew/include/GL/wglew.h"

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)

#include "../deps/glew/include/GL/glew.h"
#include <GL/gl.h>

#elif defined(___INANITY_PLATFORM_MACOS)

#include "../deps/glew/include/GL/glew.h"
#include </System/Library/Frameworks/OpenGL.framework/Headers/gl.h>

#elif defined(___INANITY_PLATFORM_SWITCH)

#include <nn/gll.h>
#include <EGL/egl.h>

#elif defined(___INANITY_PLATFORM_ANDROID)

#include "../deps/glew/include/GL/glew.h"
#include <EGL/egl.h>

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

#include <GL/glew.h>
#include <GL/gl.h>

// hacks for missing parts of Emscripten's GLEW
extern "C"
{
	void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
	void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex);
	void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor);
	void glTexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
	void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	void glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
};

#else

#error Unknown platform

#endif

#endif
