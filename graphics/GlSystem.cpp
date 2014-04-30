#include "GlSystem.hpp"
#include "GlDevice.hpp"
#include "GlContext.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "Win32Adapter.hpp"
#include "../platform/Win32Window.hpp"
#include "../Strings.hpp"
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
#include "SdlAdapter.hpp"
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
#include "EmsAdapter.hpp"
#include "../platform/EmsWindow.hpp"
#else
#error Unknown platform
#endif
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlSystem::GlSystem() : adaptersInitialized(false) {}

const std::vector<ptr<Adapter> >& GlSystem::GetAdapters()
{
	if(!adaptersInitialized)
	{
#if defined(___INANITY_PLATFORM_WINDOWS)
		Win32Adapter::GetAdapters(adapters);
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
		SdlAdapter::GetAdapters(adapters);
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
		EmsAdapter::GetAdapters(adapters);
#else
#error Unknown platform
#endif

		adaptersInitialized = true;
	}

	return adapters;
}

ptr<Device> GlSystem::CreateDevice(ptr<Adapter> abstractAdapter)
{
	BEGIN_TRY();

#if defined(___INANITY_PLATFORM_WINDOWS)
	ptr<Win32Adapter> adapter = abstractAdapter.DynamicCast<Win32Adapter>();
	if(!adapter)
		THROW("Wrong adapter type");
	return NEW(GlDevice(this, adapter->GetId()));
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_EMSCRIPTEN)
	return NEW(GlDevice(this));
#else
#error Unknown platform
#endif

	// TODO
	THROW("Not implemented");

	END_TRY("Can't create OpenGL device");
}

ptr<Context> GlSystem::CreateContext(ptr<Device> abstractDevice)
{
	BEGIN_TRY();

	ptr<GlDevice> device = abstractDevice.DynamicCast<GlDevice>();
	if(!device)
		THROW("Wrong device type");

	return NEW(GlContext(device));

	END_TRY("Can't create OpenGL context");
}

void GlSystem::InitGLEW()
{
	GLenum err = glewInit();
	if(err != GLEW_OK)
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
		THROW("Can't initialize GLEW");
#else
		THROW(String("Can't initialize GLEW: ") + (const char*)glewGetErrorString(err));
#endif
}

void GlSystem::ClearErrors()
{
	while(glGetError() != GL_NO_ERROR);	
}

void GlSystem::CheckErrors(const char* primaryExceptionString)
{
#ifdef _DEBUG
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
			THROW_SECONDARY(primaryExceptionString, NEW(Exception(errorStrings)));
		else
			THROW(errorStrings);
	}
#endif
}

bool GlSystem::GetTextureFormat(PixelFormat pixelFormat, bool renderbuffer, bool& compressed, GLint& internalFormat, GLenum& format, GLenum& type)
{
#define T(t) PixelFormat::type##t
#define P(p) PixelFormat::pixel##p
#define F(f) PixelFormat::format##f
#define S(s) PixelFormat::size##s
#define C(c) PixelFormat::compression##c

	bool ok = false;
	compressed = false;

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
				case S(8bit): type = GL_UNSIGNED_BYTE; internalFormat = GL_R8; ok = true; break;
				case S(16bit): type = GL_UNSIGNED_SHORT; internalFormat = GL_R16; ok = true; break;
				default: break;
				}
				break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(16bit): type = GL_FLOAT; internalFormat = GL_R16F; ok = true; break;
				case S(32bit): type = GL_FLOAT; internalFormat = GL_R32F; ok = true; break;
				default: break;
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
				case S(16bit): type = GL_UNSIGNED_BYTE; internalFormat = GL_RG8; ok = true; break;
				case S(32bit): type = GL_UNSIGNED_SHORT; internalFormat = GL_RG16; ok = true; break;
				default: break;
				}
				break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(32bit): type = GL_FLOAT; internalFormat = GL_RG16F; ok = true; break;
				case S(64bit): type = GL_FLOAT; internalFormat = GL_RG32F; ok = true; break;
				default: break;
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
				case S(32bit): type = GL_FLOAT; internalFormat = GL_R11F_G11F_B10F; ok = true; break;
				case S(96bit): type = GL_FLOAT; internalFormat = GL_RGB32F; ok = true; break;
				default: break;
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
				case S(32bit): type = GL_UNSIGNED_BYTE; internalFormat = GL_RGBA8; ok = true; break;
				case S(64bit): type = GL_UNSIGNED_SHORT; internalFormat = GL_RGBA16; ok = true; break;
				default: break;
				}
				break;
			case F(Float):
				switch(pixelFormat.size)
				{
				case S(64bit): type = GL_FLOAT; internalFormat = GL_RGBA16F; ok = true; break;
				case S(128bit): type = GL_FLOAT; internalFormat = GL_RGBA32F; ok = true; break;
				default: break;
				}
				break;
			}
			break;
		}
		break;
	case T(Compressed):
		compressed = true;
		switch(pixelFormat.compression)
		{
		case C(Bc1):
			internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			ok = true;
			break;
		case C(Bc1Alpha):
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			ok = true;
			break;
		case C(Bc1Srgb):
			internalFormat = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
			ok = true;
			break;
		case C(Bc1SrgbAlpha):
			internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
			ok = true;
			break;
		case C(Bc2):
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			ok = true;
			break;
		case C(Bc2Srgb):
			internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
			ok = true;
			break;
		case C(Bc3):
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			ok = true;
			break;
		case C(Bc3Srgb):
			internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
			ok = true;
			break;
		case C(Bc4):
			internalFormat = GL_COMPRESSED_RED_RGTC1;
			ok = true;
			break;
		case C(Bc4Signed):
			internalFormat = GL_COMPRESSED_SIGNED_RED_RGTC1;
			ok = true;
			break;
		case C(Bc5):
			internalFormat = GL_COMPRESSED_RG_RGTC2;
			ok = true;
			break;
		case C(Bc5Signed):
			internalFormat = GL_COMPRESSED_SIGNED_RG_RGTC2;
			ok = true;
			break;
		}
		break;
	}

	if(ok)
	{
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
		if(!compressed)
		{
			if(format == GL_RED)
				format = renderbuffer ? GL_RGB : GL_LUMINANCE;
			internalFormat = format;
		}
#endif
		return true;
	}

	THROW("Pixel format is unsupported in OpenGL");
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
		THROW("Wrong attribute name");

	// перевести имя в семантику
	int semantic = 0;
	for(; i < name.length(); ++i)
	{
		char ch = name[i];
		if(ch < 'a' || ch > 'z')
			THROW("Wrong symbol in attribute semantic");
		semantic = semantic * 26 + ch - 'a';
	}

	return semantic;
}

END_INANITY_GRAPHICS
