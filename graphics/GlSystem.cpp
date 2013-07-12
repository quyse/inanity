#include "GlSystem.hpp"
#include "GlDevice.hpp"
#include "GlContext.hpp"
#include "GlShaderCompiler.hpp"
#include "shaders/GlslGenerator.hpp"
#ifdef ___INANITY_WINDOWS
#include "Win32Adapter.hpp"
#include "../platform/Win32Window.hpp"
#include "../Strings.hpp"
#endif
#ifdef ___INANITY_LINUX
#include "X11Adapter.hpp"
#include "../platform/X11Window.hpp"
#include "../platform/X11Display.hpp"
#endif
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlSystem::GlSystem() : adaptersInitialized(false) {}

const std::vector<ptr<Adapter> >& GlSystem::GetAdapters()
{
	if(!adaptersInitialized)
	{
#ifdef ___INANITY_WINDOWS
		Win32Adapter::GetAdapters(adapters);
#endif
#ifdef ___INANITY_LINUX
		X11Adapter::GetAdapters(adapters);
#endif

		adaptersInitialized = true;
	}

	return adapters;
}

ptr<Device> GlSystem::CreateDevice(ptr<Adapter> abstractAdapter)
{
	BEGIN_TRY();

#ifdef ___INANITY_WINDOWS
	ptr<Win32Adapter> adapter = abstractAdapter.DynamicCast<Win32Adapter>();
	if(!adapter)
		THROW_PRIMARY_EXCEPTION("Wrong adapter type");
	return NEW(GlDevice(this, adapter->GetId(), NEW(GlContext())));
#endif

#ifdef ___INANITY_LINUX
	return NEW(GlDevice(this, NEW(GlContext())));
#endif

	// TODO
	THROW_PRIMARY_EXCEPTION("Not implemented");

	END_TRY("Can't create OpenGL device");
}

ptr<ShaderCompiler> GlSystem::CreateShaderCompiler()
{
	return NEW(GlShaderCompiler());
}

ptr<Shaders::ShaderGenerator> GlSystem::CreateShaderGenerator()
{
	return NEW(Shaders::GlslGenerator());
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
#define T(t) PixelFormat::type##t
#define P(p) PixelFormat::pixel##p
#define F(f) PixelFormat::format##f
#define S(s) PixelFormat::size##s
#define C(c) PixelFormat::compression##c
	switch(pixelFormat.type)
	{
	case T(Unknown): break;
	case T(Uncompressed):
		switch(pixelFormat.pixel)
		{
		case P(R):
			format = GL_RED;
			switch(pixelFormat.format)
			{
			case F(Untyped): break;
			case F(Uint):
				switch(pixelFormat.size)
				{
				case S(8bit): type = GL_UNSIGNED_BYTE; internalFormat = GL_R8; return true;
				case S(16bit): type = GL_UNSIGNED_SHORT; internalFormat = GL_R16; return true;
				}
				break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(16bit): type = GL_FLOAT; internalFormat = GL_R16F; return true;
				case S(32bit): type = GL_FLOAT; internalFormat = GL_R32F; return true;
				}
				break;
			}
			break;
		case PixelFormat::pixelRG:
			format = GL_RG;
			switch(pixelFormat.format)
			{
			case F(Untyped): break;
			case F(Uint):
				switch(pixelFormat.size)
				{
				case S(16bit): type = GL_UNSIGNED_BYTE; internalFormat = GL_RG8; return true;
				case S(32bit): type = GL_UNSIGNED_SHORT; internalFormat = GL_RG16; return true;
				}
				break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(32bit): type = GL_FLOAT; internalFormat = GL_RG16F; return true;
				case S(64bit): type = GL_FLOAT; internalFormat = GL_RG32F; return true;
				}
				break;
			}
			break;
		case PixelFormat::pixelRGB:
			format = GL_RGB;
			switch(pixelFormat.format)
			{
			case F(Untyped): break;
			case F(Uint): break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(32bit): type = GL_FLOAT; internalFormat = GL_R11F_G11F_B10F; return true;
				case S(96bit): type = GL_FLOAT; internalFormat = GL_RGB32F; return true;
				}
				break;
			}
			break;
		case PixelFormat::pixelRGBA:
			format = GL_RGBA;
			switch(pixelFormat.format)
			{
			case F(Untyped): break;
			case F(Uint):
				switch(pixelFormat.size)
				{
				case S(32bit): type = GL_UNSIGNED_BYTE; internalFormat = GL_RGBA8; return true;
				case S(64bit): type = GL_UNSIGNED_SHORT; internalFormat = GL_RGBA16; return true;
				}
				break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(64bit): type = GL_FLOAT; internalFormat = GL_RGBA16F; return true;
				case S(128bit): type = GL_FLOAT; internalFormat = GL_RGBA32F; return true;
				}
				break;
			}
			break;
		}
	case T(Compressed):
		switch(pixelFormat.compression)
		{
		case C(Dxt1): format = GL_RGB; type = GL_UNSIGNED_BYTE; internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; return true;
		case C(Dxt2): format = GL_RGBA; type = GL_UNSIGNED_BYTE; internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; return true;
		case C(Dxt3): break;
		case C(Dxt4): format = GL_RGBA; type = GL_UNSIGNED_BYTE; internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; return true;
		case C(Dxt5): format = GL_RGBA; type = GL_UNSIGNED_BYTE; internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; return true;
		}
		break;
	}
	THROW_PRIMARY_EXCEPTION("Pixel format is unsupported in OpenGL");
#undef T
#undef P
#undef F
#undef S
#undef C
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

END_INANITY_GRAPHICS
