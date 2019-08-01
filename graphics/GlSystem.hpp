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
private:
	std::vector<ptr<Adapter> > adapters;
	bool adaptersInitialized;

public:
	GlSystem();

	// методы System
	const std::vector<ptr<Adapter> >& GetAdapters();
	ptr<Device> CreateDevice(ptr<Adapter> adapter);
	ptr<Context> CreateContext(ptr<Device> device);

	/// Инициализация OpenGL.
	/** Нужно вызывать после создания контекста OpenGL. */
	static void InitGL();

	/// Очистить ошибки OpenGL (игнорируя их).
	static void ClearErrors();
	/// Проверить ошибки OpenGL, и выбросить исключение, если есть.
	/**
	\param primaryExceptionString Строка с исключением, оборачивающим ошибку OpenGL (если она будет).
	Если не задана, то ошибка не оборачивается.
	*/
	static void CheckErrors(const char* primaryExceptionString = 0);

	/// Gets texture format.
	/** Returns compressed; if compressed is true, returns only internalFormat, else returns all things. */
	static bool GetTextureFormat(PixelFormat pixelFormat, bool renderbuffer, bool& compressed, GLint& internalFormat, GLenum& format, GLenum& type);

	/// Перевести имя аттрибута в спецификацию.
	static int AttributeNameToSemantic(const String& name);
};

END_INANITY_GRAPHICS

#endif
