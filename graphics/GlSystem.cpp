#include "GlSystem.hpp"
#include "GlDevice.hpp"
#include "GlContext.hpp"
#ifdef ___INANITY_WINDOWS
#include "../Win32Window.hpp"
#include "../Strings.hpp"
#endif
#include "../Exception.hpp"

ptr<Window> GlSystem::CreateDefaultWindow()
{
#ifdef ___INANITY_WINDOWS
	return Win32Window::CreateForOpenGL();
#endif
#ifdef ___INANITY_LINUX
	return NEW(X11Window());
#endif
}

ptr<Device> GlSystem::CreatePrimaryDevice()
{
	try
	{
#ifdef ___INANITY_WINDOWS
		// получить всё устройства
		DISPLAY_DEVICE deviceInfo;
		deviceInfo.cb = sizeof(deviceInfo);
		for(DWORD i = 0; EnumDisplayDevices(NULL, i, &deviceInfo, 0); ++i)
			// если устройство primary
			if(deviceInfo.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				// вернуть его
				return NEW(GlDevice(this, Strings::Unicode2UTF8(deviceInfo.DeviceName), NEW(GlContext())));

		// в MSDN написано, устройство всегда есть, так что этого быть не должно
		THROW_PRIMARY_EXCEPTION("Can't find primary device");
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create primary device", exception);
	}
}

void GlSystem::InitGLEW()
{
	GLenum err = glewInit();
	if(err != GLEW_OK)
		THROW_PRIMARY_EXCEPTION(String("Can't initialize GLEW: ") + (const char*)glewGetErrorString(err));
}

void GlSystem::ClearErrors()
{
	while(glGetError() != GL_NO_ERROR);	
}

void GlSystem::CheckErrors(const char* primaryExceptionString)
{
	GLenum error = glGetError();
	// если есть хотя бы одна ошибка
	if(error != GL_NO_ERROR)
	{
		// согласно спецификации, может быть несколько ошибок
		// получаем их, пока не кончатся
		String errorStrings = "OpenGL error(s):";
		do
		{
			// получить строку ошибки
			const char* errorString;

#define DEFINE_GL_ERROR(code) \
	case code: \
		errorString = " " #code; \
		break

			switch(error)
			{
				DEFINE_GL_ERROR(GL_INVALID_ENUM);
				DEFINE_GL_ERROR(GL_INVALID_VALUE);
				DEFINE_GL_ERROR(GL_INVALID_OPERATION);
				DEFINE_GL_ERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
				DEFINE_GL_ERROR(GL_OUT_OF_MEMORY);
			default:
				errorString = "Unknown OpenGL error";
				break;
			}

#undef DEFINE_GL_ERROR

			errorStrings += errorString;
		}
		while((error = glGetError()) != GL_NO_ERROR);

		// всё, ошибки кончились, и флаги ошибок очищены
		// бросить исключение
		if(primaryExceptionString)
			THROW_SECONDARY_EXCEPTION(primaryExceptionString, NEW(Exception(errorStrings)));
		else
			THROW_PRIMARY_EXCEPTION(errorStrings);
	}
}

bool GlSystem::GetTextureFormat(PixelFormat pixelFormat, GLint& internalFormat, GLenum& format, GLenum& type)
{
	switch(pixelFormat)
	{
	case PixelFormats::intR8G8B8A8:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		return true;
	case PixelFormats::floatR11G11B10:
		internalFormat = GL_R11F_G11F_B10F;
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;
		return true;
	//case PixelFormats::typelessR32:
	case PixelFormats::floatR32:
		internalFormat = GL_R32F;
		format = GL_RED;
		type = GL_UNSIGNED_BYTE;
		return true;
	case PixelFormats::floatR32Depth:
		internalFormat = GL_DEPTH_COMPONENT32F;
		format = GL_DEPTH_COMPONENT;
		type = GL_FLOAT;
		return true;
	case PixelFormats::unknown:
	default:
		return false;
	}
}

int GlSystem::AttributeNameToSemantic(const String& name)
{
	// имя атрибута должно начинаться с "attr_"
	static const char beginStr[] = "attr_";
	size_t i;
	for(i = 0; i < name.length() && beginStr[i]; ++i)
		if(name[i] != beginStr[i])
			break;
	if(beginStr[i])
		THROW_PRIMARY_EXCEPTION("Wrong attribute name");

	// перевести имя в семантику
	int semantic = 0;
	for(; i < name.length(); ++i)
	{
		char ch = name[i];
		if(ch < 'a' || ch > 'z')
			THROW_PRIMARY_EXCEPTION("Wrong symbol in attribute semantic");
		semantic = semantic * 26 + ch - 'a';
	}

	return semantic;
}