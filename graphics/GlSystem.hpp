#ifndef ___INANITY_GRAPHICS_GL_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_GL_SYSTEM_HPP___

#include "System.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс графической системы OpenGL.
class GlSystem : public System
{
public:
	// методы System
	ptr<Window> CreateWindow();
	ptr<Device> CreatePrimaryDevice();

	/// Проверить ошибки OpenGL, и выбросить исключение, если есть.
	/**
	\param primaryExceptionString Строка с исключением, оборачивающим ошибку OpenGL (если она будет).
	Если не задана, то ошибка не оборачивается.
	*/
	static void CheckErrors(const char* primaryExceptionString = 0);
	/// Получить формат текстуры для OpenGL.
	static bool GetTextureFormat(PixelFormat pixelFormat, GLint& internalFormat, GLenum& format, GLenum& type);
};

END_INANITY_GRAPHICS

#endif
