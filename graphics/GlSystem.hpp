#ifndef ___INANITY_GRAPHICS_GL_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_GL_SYSTEM_HPP___

#include "System.hpp"
#include "PixelFormat.hpp"
#include "opengl.hpp"
#include "../String.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс графической системы OpenGL.
class GlSystem : public System
{
public:
	// методы System
	ptr<Window> CreateDefaultWindow();
	ptr<Device> CreatePrimaryDevice();
	ptr<ShaderCompiler> CreateShaderCompiler();
	ptr<Shaders::ShaderGenerator> CreateShaderGenerator();

	/// Инициализация GLEW.
	/** Нужно вызывать после создания контекста OpenGL. */
	static void InitGLEW();

	/// Очистить ошибки OpenGL (игнорируя их).
	static void ClearErrors();
	/// Проверить ошибки OpenGL, и выбросить исключение, если есть.
	/**
	\param primaryExceptionString Строка с исключением, оборачивающим ошибку OpenGL (если она будет).
	Если не задана, то ошибка не оборачивается.
	*/
	static void CheckErrors(const char* primaryExceptionString = 0);

	/// Получить формат текстуры для OpenGL.
	static bool GetTextureFormat(PixelFormat pixelFormat, GLint& internalFormat, GLenum& format, GLenum& type);

	/// Перевести имя аттрибута в спецификацию.
	static int AttributeNameToSemantic(const String& name);
};

END_INANITY_GRAPHICS

#endif
