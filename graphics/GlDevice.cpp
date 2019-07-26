#include "GlDevice.hpp"
#include "GlSystem.hpp"
#include "GlShaderCompiler.hpp"
#include "shaders/GlslGenerator.hpp"
#include "GlFrameBuffer.hpp"
#include "GlRenderBuffer.hpp"
#include "GlDepthStencilBuffer.hpp"
#include "GlContext.hpp"
#include "GlTexture.hpp"
#include "GlVertexShader.hpp"
#include "GlPixelShader.hpp"
#include "GlUniformBuffer.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "VertexLayout.hpp"
#include "GlAttributeBinding.hpp"
#include "AttributeLayout.hpp"
#include "GlInternalTexture.hpp"
#include "RawTextureData.hpp"
#include "GlSamplerState.hpp"
#include "GlDepthStencilState.hpp"
#include "GlBlendState.hpp"
#include "GlslSource.hpp"
#include "GlShaderBindings.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"
#include "../Strings.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "WglPresenter.hpp"
#include "Win32MonitorMode.hpp"
#include "../platform/Win32Window.hpp"
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)
#include "SdlPresenter.hpp"
#include "SdlMonitorMode.hpp"
#include "../platform/Sdl.hpp"
#include "../platform/SdlWindow.hpp"
#elif defined(___INANITY_PLATFORM_SWITCH)
#include "NxPresenter.hpp"
#include "../platform/NxWindow.hpp"
#include "MonitorMode.hpp"
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
#include "EmsPresenter.hpp"
#include "MonitorMode.hpp"
#include "../platform/EmsWindow.hpp"
#include <emscripten/emscripten.h>
#else
#error Unknown platform
#endif

BEGIN_INANITY_GRAPHICS

#if defined(___INANITY_PLATFORM_WINDOWS)

GlDevice::GlDevice(ptr<GlSystem> system, const String& deviceName)
: system(system), deviceName(deviceName), boundPresenter(nullptr), hglrc(0)
{
	// create hidden window
	hiddenWindow = Platform::Win32Window::CreateForOpenGL("Inanity OpenGL Hidden Window",
		0, 0, // left top
		1, 1, // width height
		false // visible
	);

	// get window's HDC
	HDC hdc = hiddenWindow->GetHDC();

	// create temporary context and make it current
	HGLRC hglrcTemp = wglCreateContext(hdc);
	if(!wglMakeCurrent(hdc, hglrcTemp))
		THROW_SECONDARY("Can't make temp OpenGL context current", Exception::SystemError());

	// create real context
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 0,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0
#ifdef _DEBUG
			| WGL_CONTEXT_DEBUG_BIT_ARB
#endif
		,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0, 0
	};
	// versions to try
	static const int versions[][2] =
	{
		{ 4, 3 },
		{ 4, 2 },
		{ 4, 1 },
		{ 4, 0 },
		{ 3, 3 },
		{ 3, 2 }
	};
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if(!wglCreateContextAttribsARB)
		THROW("Can't get wglCreateContextAttribsARB");
	// loop for versions
	for(int i = 0; i < sizeof(versions) / sizeof(versions[0]); ++i)
	{
		attribs[1] = versions[i][0];
		attribs[3] = versions[i][1];
		hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
		if(hglrc)
			break;
	}
	// if no version is supported, bummer
	if(!hglrc)
		THROW("Can't create OpenGL window context");

	// make new context current
	if(!wglMakeCurrent(hdc, hglrc))
		THROW_SECONDARY("Can't make OpenGL context current", Exception::SystemError());

	// delete temporary context
	wglDeleteContext(hglrcTemp);

	// initialize
	GlSystem::Init();

	// init capabilities
	InitCaps();

	// clear errors after GLEW
	GlSystem::ClearErrors();
}

GlDevice::~GlDevice()
{
	if(hglrc)
		wglDeleteContext(hglrc);
}

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)

GlDevice::GlDevice(ptr<GlSystem> system)
: system(system), boundPresenter(nullptr), sdlContext(0)
{
	BEGIN_TRY();

	// create hidden window
	{
#if defined(___INANITY_PLATFORM_MACOS)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

		SDL_Window* handle = SDL_CreateWindow("Inanity OpenGL Hidden Window",
			SDL_WINDOWPOS_CENTERED, // x
			SDL_WINDOWPOS_CENTERED, // y
			1,
			1,
			SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN);
		if(!handle)
			THROW("Can't create temp window");
		hiddenWindow = NEW(Platform::SdlWindow(handle));
	}

	// create OpenGL context for hidden window
	sdlContext = SDL_GL_CreateContext(hiddenWindow->GetHandle());
	if(!sdlContext)
		THROW_SECONDARY("Can't create OpenGL context", Platform::Sdl::Error());

	// initialize extensions
	GlSystem::Init();
	GlSystem::ClearErrors();

	// init capabilities
	InitCaps();

	END_TRY("Can't create OpenGL device");
}

GlDevice::~GlDevice()
{
	if(sdlContext)
		SDL_GL_DeleteContext(sdlContext);
}

#elif defined(___INANITY_PLATFORM_SWITCH)

GlDevice::GlDevice(ptr<GlSystem> system)
: system(system) {}

GlDevice::~GlDevice() {}

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

GlDevice::GlDevice(ptr<GlSystem> system)
: system(system) {}

GlDevice::~GlDevice() {}

#else

#error Unknown platform

#endif

void GlDevice::InitCaps()
{
#if defined(___INANITY_PLATFORM_WINDOWS) || defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)
	GLint majorVersion, minorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	GLint version = majorVersion * 100 + minorVersion;
	internalCaps =
		((version >= 301 || GLEW_ARB_uniform_buffer_object) ? InternalCaps::uniformBufferObject : 0) |
		((version >= 303 || GLEW_ARB_sampler_objects) ? InternalCaps::samplerObjects : 0) |
		((version >= 300 || GLEW_ARB_vertex_array_object) ? InternalCaps::vertexArrayObject : 0) |
		((version >= 403 || GLEW_ARB_vertex_attrib_binding) ? InternalCaps::vertexAttribBinding : 0) |
		((version >= 300 || GLEW_ARB_framebuffer_object) ? InternalCaps::frameBufferObject : 0) |
		((version >= 402 || GLEW_ARB_texture_storage) ? InternalCaps::textureStorage : 0)
		;
	caps.flags =
		((version >= 303 || GLEW_ARB_instanced_arrays) ? Caps::attributeInstancing : 0) |
		((version >= 301 || GLEW_ARB_draw_instanced) ? Caps::drawInstancing : 0);
	caps.maxColorBuffersCount = GlFrameBuffer::maxColorBuffersCount;
#elif defined(___INANITY_PLATFORM_SWITCH)
	internalCaps =
		InternalCaps::uniformBufferObject |
		InternalCaps::samplerObjects |
		InternalCaps::vertexArrayObject |
		InternalCaps::vertexAttribBinding |
		InternalCaps::frameBufferObject |
		InternalCaps::textureStorage
		;
	caps.flags =
		Caps::attributeInstancing |
		Caps::drawInstancing
		;
	caps.maxColorBuffersCount = GlFrameBuffer::maxColorBuffersCount;
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
	internalCaps = InternalCaps::frameBufferObject;
	caps.flags = 0;
	caps.maxColorBuffersCount = 1;
#else
#error Unknown platform
#endif
}

int GlDevice::GetInternalCaps() const
{
	return internalCaps;
}

void GlDevice::BindPresenter(Presenter* presenter)
{
	if(boundPresenter != presenter)
	{
		BEGIN_TRY();

		// switch by type of presenter
#if defined(___INANITY_PLATFORM_WINDOWS)

		if(presenter)
		{
			WglPresenter* wglPresenter = dynamic_cast<WglPresenter*>(presenter);
			if(wglPresenter)
				wglPresenter->Bind(hglrc);
			else
				THROW("Unsupported type of presenter");
		}
		else
		{
			// bind hidden window
			if(!wglMakeCurrent(hiddenWindow->GetHDC(), hglrc))
				THROW("Can't bind hidden window");
		}

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)

		if(presenter)
		{
			SdlPresenter* sdlPresenter = dynamic_cast<SdlPresenter*>(presenter);
			if(sdlPresenter)
				sdlPresenter->Bind(sdlContext);
			else
				THROW("Unsupported type of presenter");
		}
		else
		{
			// bind hidden window
			if(SDL_GL_MakeCurrent(hiddenWindow->GetHandle(), sdlContext) != 0)
				THROW("Can't bind hidden window");
		}

#elif defined(___INANITY_PLATFORM_SWITCH)

		// TODO

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

		// no binding needed for Emscripten presenter

#else
#error Unknown platform
#endif

		boundPresenter = presenter;

		END_TRY("Can't bind presenter to OpenGL context");
	}
}

void GlDevice::UnbindPresenter(Presenter* presenter)
{
	if(boundPresenter == presenter)
		BindPresenter(nullptr);
}

ptr<System> GlDevice::GetSystem() const
{
	return system;
}

ptr<Presenter> GlDevice::CreateWindowPresenter(ptr<Platform::Window> abstractWindow, ptr<MonitorMode> abstractMode)
{
	BEGIN_TRY();

#if defined(___INANITY_PLATFORM_WINDOWS)

	ptr<Platform::Win32Window> window = abstractWindow.FastCast<Platform::Win32Window>();
	if(!window)
		THROW("Only Win32 window is allowed");
	ptr<Win32MonitorMode> mode = abstractMode.FastCast<Win32MonitorMode>();
	if(!mode && abstractMode)
		THROW("Only Win32 monitor mode allowed");

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)

	ptr<Platform::SdlWindow> window = abstractWindow.FastCast<Platform::SdlWindow>();
	if(!window)
		THROW("Only SDL window is allowed");
	ptr<SdlMonitorMode> mode = abstractMode.FastCast<SdlMonitorMode>();
	if(!mode && abstractMode)
		THROW("Only SDL monitor mode is allowed");

#elif defined(___INANITY_PLATFORM_SWITCH)

	ptr<Platform::NxWindow> window = abstractWindow.FastCast<Platform::NxWindow>();
	if(!window)
		THROW("Only Nx window is allowed");
	ptr<MonitorMode> mode = nullptr;

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

	ptr<Platform::EmsWindow> window = abstractWindow.FastCast<Platform::EmsWindow>();
	if(!window)
		THROW("Only Emscripten window is allowed");
	ptr<MonitorMode> mode = abstractMode;

#else
#error Unknown platform
#endif

	return CreatePresenter(window, mode);

	END_TRY("Can't create OpenGL window presenter");
}

#if defined(___INANITY_PLATFORM_WINDOWS)

ptr<WglPresenter> GlDevice::CreatePresenter(ptr<Platform::Win32Window> window, ptr<Win32MonitorMode> mode)
{
	BEGIN_TRY();

	ptr<GlFrameBuffer> frameBuffer = NEW(GlFrameBuffer(this, 0));
	ptr<WglPresenter> presenter = NEW(WglPresenter(this, frameBuffer, GetDC(window->GetHWND()), window));
	frameBuffer->SetPresenter(presenter);
	return presenter;

	END_TRY("Can't create Windows OpenGL presenter");
}

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD) || defined(___INANITY_PLATFORM_MACOS) || defined(___INANITY_PLATFORM_ANDROID)

ptr<SdlPresenter> GlDevice::CreatePresenter(ptr<Platform::SdlWindow> window, ptr<SdlMonitorMode> mode)
{
	BEGIN_TRY();

	ptr<GlFrameBuffer> frameBuffer = NEW(GlFrameBuffer(this, 0));
	ptr<SdlPresenter> presenter = NEW(SdlPresenter(this, frameBuffer, window));
	frameBuffer->SetPresenter(presenter);
	return presenter;

	END_TRY("Can't create SDL presenter");
}

#elif defined(___INANITY_PLATFORM_SWITCH)

ptr<NxPresenter> GlDevice::CreatePresenter(ptr<Platform::NxWindow> window, ptr<MonitorMode> mode)
{
	BEGIN_TRY();

	ptr<GlFrameBuffer> frameBuffer = NEW(GlFrameBuffer(this, 0));
	return NEW(NxPresenter(this, frameBuffer, window));

	END_TRY("Can't create Nx presenter");
}

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

ptr<EmsPresenter> GlDevice::CreatePresenter(ptr<Platform::EmsWindow> window, ptr<MonitorMode> mode)
{
	BEGIN_TRY();

	// init capabilities
	InitCaps();

	return NEW(EmsPresenter(this, NEW(GlFrameBuffer(this, 0)), window));

	END_TRY("Can't create Emscripten presenter");
}

#else

#error Unknown platform

#endif

ptr<ShaderCompiler> GlDevice::CreateShaderCompiler()
{
	return NEW(GlShaderCompiler());
}

ptr<Shaders::ShaderGenerator> GlDevice::CreateShaderGenerator()
{
	Shaders::GlslVersion glslVersion =
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
		Shaders::GlslVersions::webgl
#else
		Shaders::GlslVersions::opengl33
#endif
		;
	return NEW(Shaders::GlslGenerator(glslVersion, internalCaps & InternalCaps::uniformBufferObject));
}

ptr<FrameBuffer> GlDevice::CreateFrameBuffer()
{
	GLuint name;
	glGenFramebuffers(1, &name);
	GlSystem::CheckErrors("Can't create OpenGL framebuffer");
	return NEW(GlFrameBuffer(this, name));
}

ptr<RenderBuffer> GlDevice::CreateRenderBuffer(int width, int height, PixelFormat pixelFormat, const SamplerSettings& samplerSettings)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(this, textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		bool compressed;
		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(pixelFormat, true, compressed, internalFormat, format, type))
			THROW("Invalid pixel format");

		if(compressed)
			THROW("Compressed format can't be used for render buffer");

		// set texture storage
		if(internalCaps & InternalCaps::textureStorage)
			glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, (GLsizei)width, (GLsizei)height);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)width, (GLsizei)height, 0, format, type, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		// correct sampler settings
		SamplerSettings ss = samplerSettings;
		ss.mipMapping = false;
#if defined(___INANITY_PLATFORM_EMSCRIPTEN)
		// disable mipmapping and set clamp wrapping mode for NPOT texture
		if((width & (width - 1)) || (height & (height - 1)))
			ss.SetWrap(SamplerSettings::wrapClamp);
#endif

		SetupTextureSampling(GL_TEXTURE_2D, ss);

		return NEW(GlRenderBuffer(internalTexture, NEW(GlTexture(internalTexture, width, height, 0))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create render buffer", exception);
	}
}

ptr<DepthStencilBuffer> GlDevice::CreateDepthStencilBuffer(int width, int height, bool canBeResource)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen textures");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(this, textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlDepthStencilBuffer(internalTexture, NEW(GlTexture(internalTexture, width, height, 0)), width, height));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create depth stencil buffer", exception);
	}
}

void GlDevice::CompileShader(GLuint shaderName, ptr<File> file, ptr<GlShaderBindings>& shaderBindings)
{
	// десериализовать исходник
	ptr<GlslSource> source = GlslSource::Deserialize(NEW(FileInputStream(file)));

	// вернуть привязки
	shaderBindings = source->GetShaderBindings();

	// указать текст шейдера
	ptr<File> code = source->GetCode();
	const GLchar* string = (const GLchar*)code->GetData();
	const GLint length = (GLint)code->GetSize();
	glShaderSource(shaderName, 1, &string, &length);
	GlSystem::CheckErrors("Can't set shader source");

	// скомпилировать шейдер
	glCompileShader(shaderName);
	{
		GLint status;
		glGetShaderiv(shaderName, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			// получить лог ошибок
			GLint logLength;
			glGetShaderiv(shaderName, GL_INFO_LOG_LENGTH, &logLength);
			String log(logLength, ' ');
			glGetShaderInfoLog(shaderName, logLength, &logLength, &*log.begin());
			log.resize(logLength);

			// очистить ошибки, на всякий случай
			GlSystem::ClearErrors();

			// выбросить ошибку
			THROW("Can't compile shader:\n" + Strings::File2String(code) + "\n" + log);
		}
	}
}

ptr<VertexShader> GlDevice::CreateVertexShader(ptr<File> file)
{
	try
	{
		GLuint shaderName = glCreateShader(GL_VERTEX_SHADER);
		GlSystem::CheckErrors("Can't create shader");

		ptr<GlShaderBindings> shaderBindings;
		CompileShader(shaderName, file, shaderBindings);

		return NEW(GlVertexShader(this, shaderName, shaderBindings));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create vertex shader", exception);
	}
}

ptr<PixelShader> GlDevice::CreatePixelShader(ptr<File> file)
{
	try
	{
		GLuint shaderName = glCreateShader(GL_FRAGMENT_SHADER);
		GlSystem::CheckErrors("Can't create shader");

		ptr<GlShaderBindings> shaderBindings;
		CompileShader(shaderName, file, shaderBindings);

		return NEW(GlPixelShader(this, shaderName, shaderBindings));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create pixel shader", exception);
	}
}

ptr<UniformBuffer> GlDevice::CreateUniformBuffer(int size)
{
	try
	{
		// if hardware uniform buffers are supported
		if(internalCaps & InternalCaps::uniformBufferObject)
		{
			GLuint bufferName;
			glGenBuffers(1, &bufferName);
			GlSystem::CheckErrors("Can't gen buffer");
			ptr<GlUniformBuffer> uniformBuffer = NEW(GlUniformBuffer(this, bufferName, size));

			glBindBuffer(GL_UNIFORM_BUFFER, bufferName);
			GlSystem::CheckErrors("Can't bind buffer");

			glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
			GlSystem::CheckErrors("Can't setup buffer data");

			return uniformBuffer;
		}
		// else create emulated uniform buffer
		else
			return NEW(GlUniformBuffer(this, 0, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create uniform buffer", exception);
	}
}

ptr<VertexBuffer> GlDevice::CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlVertexBuffer> vertexBuffer = NEW(GlVertexBuffer(this, bufferName, (int)(file->GetSize() / layout->GetStride()), layout));

		glBindBuffer(GL_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ARRAY_BUFFER, file->GetSize(), file->GetData(), GL_STATIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return vertexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create OpenGL static vertex buffer", exception);
	}
}

ptr<VertexBuffer> GlDevice::CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlVertexBuffer> vertexBuffer = NEW(GlVertexBuffer(this, bufferName, size / layout->GetStride(), layout));

		glBindBuffer(GL_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return vertexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create OpenGL dynamic vertex buffer", exception);
	}
}

ptr<IndexBuffer> GlDevice::CreateStaticIndexBuffer(ptr<File> file, int indexSize)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlIndexBuffer> indexBuffer = NEW(GlIndexBuffer(this, bufferName, (int)(file->GetSize() / indexSize), indexSize));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, file->GetSize(), file->GetData(), GL_STATIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return indexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create OpenGL static index buffer", exception);
	}
}

ptr<IndexBuffer> GlDevice::CreateDynamicIndexBuffer(int size, int indexSize)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlIndexBuffer> indexBuffer = NEW(GlIndexBuffer(this, bufferName, size / indexSize, indexSize));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return indexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create OpenGL dynamic index buffer", exception);
	}
}

void GlDevice::GetAttributeSizeAndType(DataType dataType, LayoutDataType layoutDataType, GLint& size, GLenum& type, bool& integer)
{
	switch(dataType)
	{
	case DataTypes::_float:
		size = 1;
		integer = false;
		break;
	case DataTypes::_vec2:
		size = 2;
		integer = false;
		break;
	case DataTypes::_vec3:
		size = 3;
		integer = false;
		break;
	case DataTypes::_vec4:
		size = 4;
		integer = false;
		break;
	case DataTypes::_mat4x4:
		THROW("Matrices can't be used in attributes");
	case DataTypes::_uint:
	case DataTypes::_int:
		size = 1;
		integer = true;
		break;
	case DataTypes::_uvec2:
	case DataTypes::_ivec2:
		size = 2;
		integer = true;
		break;
	case DataTypes::_uvec3:
	case DataTypes::_ivec3:
		size = 3;
		integer = true;
		break;
	case DataTypes::_uvec4:
	case DataTypes::_ivec4:
		size = 4;
		integer = true;
		break;
	default:
		THROW("Unknown data type");
	}

	switch(layoutDataType)
	{
	case LayoutDataTypes::Float32:
		type = GL_FLOAT;
		break;
	case LayoutDataTypes::Uint32:
		type = GL_UNSIGNED_INT;
		break;
	case LayoutDataTypes::Uint16:
		type = GL_UNSIGNED_SHORT;
		break;
	case LayoutDataTypes::Uint8:
		type = GL_UNSIGNED_BYTE;
		break;
	case LayoutDataTypes::Int32:
		type = GL_INT;
		break;
	case LayoutDataTypes::Int16:
		type = GL_SHORT;
		break;
	case LayoutDataTypes::Int8:
		type = GL_BYTE;
		break;
	default:
		THROW("Unknown layout data type");
	}
}

ptr<AttributeBinding> GlDevice::CreateAttributeBinding(ptr<AttributeLayout> layout)
{
	try
	{
		// if vertex arrays are supported, we have to create one
		GLuint vertexArrayName = 0;
		if(internalCaps & InternalCaps::vertexArrayObject)
		{
			// создать Vertex Array Object
			glGenVertexArrays(1, &vertexArrayName);
			GlSystem::CheckErrors("Can't gen vertex array");

			// привязать VAO
			glBindVertexArray(vertexArrayName);
			GlSystem::CheckErrors("Can't bind vertex array");
		}

		// create binding
		ptr<GlAttributeBinding> binding = NEW(GlAttributeBinding(this, vertexArrayName));

		// if both arrays and attrib bindings are supported, we can create binding now
		if(vertexArrayName && (internalCaps & InternalCaps::vertexAttribBinding))
		{
			const AttributeLayout::Elements& elements = layout->GetElements();
			const AttributeLayout::Slots& slots = layout->GetSlots();

			// задать элементы в VAO
			for(size_t i = 0; i < elements.size(); ++i)
			{
				const AttributeLayout::Element& element = elements[i];

				glEnableVertexAttribArray((GLuint)i);
				GlSystem::CheckErrors("Can't enable vertex attribute array");

				// выбрать формат и размер
				GLint size;
				GLenum type;
				bool integer;
				GetAttributeSizeAndType(element.dataType, element.layoutDataType, size, type, integer);

				if(integer)
					glVertexAttribIFormat((GLuint)i, size, type, element.offset);
				else
					glVertexAttribFormat((GLuint)i, size, type, false, element.offset);
				GlSystem::CheckErrors("Can't set vertex attribute format");

				// указать слот для элемента
				glVertexAttribBinding((GLuint)i, element.slot);
			}

			// задать разделители для слотов
			for(size_t i = 0; i < slots.size(); ++i)
			{
				glVertexBindingDivisor((GLuint)i, slots[i].divisor);
				GlSystem::CheckErrors("Can't set vertex binding divisor");
			}
		}
		// иначе использовать "ручную" привязку
		else
		{
			const AttributeLayout::Elements& elements = layout->GetElements();
			const AttributeLayout::Slots& slots = layout->GetSlots();

			GlAttributeBinding::Slots& bindingSlots = binding->GetSlots();

			// задать слоты
			bindingSlots.resize(slots.size());
			for(size_t i = 0; i < slots.size(); ++i)
				bindingSlots[i].divisor = slots[i].divisor;

			// задать элементы
			for(size_t i = 0; i < elements.size(); ++i)
			{
				const AttributeLayout::Element& element = elements[i];

				GlAttributeBinding::Slot& bindingSlot = bindingSlots[element.slot];

				GlAttributeBinding::Element bindingElement;
				bindingElement.index = (GLuint)i;
				GetAttributeSizeAndType(element.dataType, element.layoutDataType, bindingElement.size, bindingElement.type, bindingElement.integer);
				bindingElement.normalized = false;
				bindingElement.pointer = (GLvoid*)(size_t)element.offset;

				bindingSlot.elements.push_back(bindingElement);
			}
		}

		// unbind VAO if needed
		if(vertexArrayName)
		{
			glBindVertexArray(0);
			GlSystem::CheckErrors("Can't unbind vertex array");
		}

		return binding;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create GL attribute binding", exception);
	}
}

/// Convert abstracted wrap into OpenGL enum.
GLint GlDevice::ConvertSamplerSettingsWrap(SamplerSettings::Wrap wrap)
{
	GLint r;
	switch(wrap)
	{
	case SamplerSettings::wrapRepeat:
		r = GL_REPEAT;
		break;
	case SamplerSettings::wrapRepeatMirror:
		r = GL_MIRRORED_REPEAT;
		break;
	case SamplerSettings::wrapClamp:
		r = GL_CLAMP_TO_EDGE;
		break;
	case SamplerSettings::wrapBorder:
		r = GL_CLAMP_TO_BORDER;
		break;
	default:
		THROW("Invalid wrap mode");
	}

#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	// GL_CLAMP_TO_BORDER is unsupported in WebGL
	if(r == GL_CLAMP_TO_BORDER)
		r = GL_CLAMP_TO_EDGE;
#endif

	return r;
}

void GlDevice::ConvertSamplerSettingsFilters(const SamplerSettings& samplerSettings, GLint& minFilter, GLint& magFilter)
{
	// get min filter
	minFilter = -1;
	if(samplerSettings.mipMapping)
		switch(samplerSettings.minFilter)
		{
		case SamplerSettings::filterPoint:
			switch(samplerSettings.mipFilter)
			{
			case SamplerSettings::filterPoint:
				minFilter = GL_NEAREST_MIPMAP_NEAREST;
				break;
			case SamplerSettings::filterLinear:
				minFilter = GL_NEAREST_MIPMAP_LINEAR;
				break;
			default:
				break;
			}
			break;
		case SamplerSettings::filterLinear:
			switch(samplerSettings.mipFilter)
			{
			case SamplerSettings::filterPoint:
				minFilter = GL_LINEAR_MIPMAP_NEAREST;
				break;
			case SamplerSettings::filterLinear:
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	else
		switch(samplerSettings.minFilter)
		{
		case SamplerSettings::filterPoint:
			minFilter = GL_NEAREST;
			break;
		case SamplerSettings::filterLinear:
			minFilter = GL_LINEAR;
			break;
		default:
			break;
		}
	if(minFilter == -1)
		THROW("Invalid min or mip filter");

	// get mag filter
	magFilter = -1;
	switch(samplerSettings.magFilter)
	{
	case SamplerSettings::filterPoint:
		magFilter = GL_NEAREST;
		break;
	case SamplerSettings::filterLinear:
		magFilter = GL_LINEAR;
		break;
	default:
		break;
	}
	if(magFilter == -1)
		THROW("Invalid mag filter");
}

void GlDevice::SetupTextureSampling(GLenum target, const SamplerSettings& samplerSettings)
{
	GLint minFilter, magFilter;
	ConvertSamplerSettingsFilters(samplerSettings, minFilter, magFilter);
	// set min filter
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
	GlSystem::CheckErrors("Can't set texture min filter");
	// set mag filter
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
	GlSystem::CheckErrors("Can't set texture mag filter");

	// set wrapping modes
	glTexParameteri(target, GL_TEXTURE_WRAP_S, ConvertSamplerSettingsWrap(samplerSettings.wrapU));
	GlSystem::CheckErrors("Can't set texture wrap U");
	glTexParameteri(target, GL_TEXTURE_WRAP_T, ConvertSamplerSettingsWrap(samplerSettings.wrapV));
	GlSystem::CheckErrors("Can't set texture wrap V");
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
	glTexParameteri(target, GL_TEXTURE_WRAP_R, ConvertSamplerSettingsWrap(samplerSettings.wrapW));
	GlSystem::CheckErrors("Can't set texture wrap W");

	// set min and max LOD.
	glTexParameterf(target, GL_TEXTURE_MIN_LOD, samplerSettings.minLOD);
	GlSystem::CheckErrors("Can't set min LOD");
	glTexParameterf(target, GL_TEXTURE_MAX_LOD, samplerSettings.maxLOD);
	GlSystem::CheckErrors("Can't set max LOD");

	// set border color
	glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, samplerSettings.borderColor.t);
	GlSystem::CheckErrors("Can't set border color");
#endif
}

ptr<Texture> GlDevice::CreateStaticTexture(ptr<RawTextureData> data, const SamplerSettings& samplerSettings)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(this, textureName));

		int mips = data->GetImageMips();
		int realCount = data->GetCount();
		if(realCount == 0)
			realCount = 1;

		bool compressed;
		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(data->GetFormat(), false, compressed, internalFormat, format, type))
			THROW("Invalid pixel format");

		int pixelSize = PixelFormat::GetPixelSize(data->GetFormat().size);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLenum target;

		bool useTextureStorage = !!(internalCaps & InternalCaps::textureStorage);

		int imageDepth = data->GetImageDepth();
		int imageHeight = data->GetImageHeight();
		int imageWidth = data->GetImageWidth();
		int imageCount = data->GetCount();

		// 3D texture
		if(imageDepth)
		{
			// if array
			if(imageCount)
				THROW("Arrays of 3D textures are not supported");

			target = GL_TEXTURE_3D;

			glBindTexture(target, textureName);
			GlSystem::CheckErrors("Can't bind 3D texture");

			if(useTextureStorage)
				glTexStorage3D(target, mips, internalFormat, imageWidth, imageHeight, imageDepth);

			for(int mip = 0; mip < mips; ++mip)
			{
				int linePitch = data->GetMipLinePitch(mip);
				int slicePitch = data->GetMipSlicePitch(mip);
				if(pixelSize && ((linePitch % pixelSize) || (slicePitch % pixelSize)))
					THROW("Wrong line or slice pitch");
				glPixelStorei(GL_UNPACK_ROW_LENGTH, pixelSize ? linePitch / pixelSize : 0);
				glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, pixelSize ? slicePitch / pixelSize : 0);

				int mipWidth = data->GetMipWidth(mip);
				int mipHeight = data->GetMipHeight(mip);
				int mipDepth = data->GetMipDepth(mip);
				int mipSize = data->GetMipSize(mip);
				const void* mipData = data->GetMipData(0, mip);

				if(useTextureStorage)
				{
					if(compressed)
					{
						glCompressedTexSubImage3D(target, mip, 0, 0, 0, mipWidth, mipHeight, mipDepth, internalFormat, mipSize, mipData);
					}
					else
					{
						glTexSubImage3D(target, mip, 0, 0, 0, mipWidth, mipHeight, mipDepth, format, type, mipData);
					}
				}
				else
				{
					if(compressed)
					{
						glCompressedTexImage3D(target, mip, internalFormat, mipWidth, mipHeight, mipDepth, 0, mipSize, mipData);
					}
					else
					{
						glTexImage3D(target, mip, internalFormat, mipWidth, mipHeight, mipDepth, 0, format, type, mipData);
					}
				}

				GlSystem::CheckErrors("Can't set 3D texture data");
			}
		}
		// 2D texture
		else if(imageHeight)
		{
			// if array
			if(imageCount)
			{
				target = GL_TEXTURE_2D_ARRAY;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 2D array texture");

				if(useTextureStorage)
					glTexStorage3D(target, mips, internalFormat, imageWidth, imageHeight, imageCount);

				int imagePitch = data->GetImageSize();
				if(pixelSize && imagePitch % pixelSize)
					THROW("Wrong image pitch");
				glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, pixelSize ? imagePitch / pixelSize : 0);
				for(int mip = 0; mip < mips; ++mip)
				{
					int linePitch = data->GetMipLinePitch(mip);
					if(pixelSize && linePitch % pixelSize)
						THROW("Wrong line pitch");
					glPixelStorei(GL_UNPACK_ROW_LENGTH, pixelSize ? linePitch / pixelSize : 0);

					int mipWidth = data->GetMipWidth(mip);
					int mipHeight = data->GetMipHeight(mip);
					int mipSize = data->GetMipSize(mip);
					const void* mipData = data->GetMipData(0, mip);

					if(useTextureStorage)
					{
						if(compressed)
						{
							glCompressedTexSubImage3D(target, mip, 0, 0, 0, mipWidth, mipHeight, imageCount, internalFormat, mipSize, mipData);
						}
						else
						{
							glTexSubImage3D(target, mip, 0, 0, 0, mipWidth, mipHeight, imageCount, format, type, mipData);
						}
					}
					else
					{
						if(compressed)
						{
							glCompressedTexImage3D(target, mip, internalFormat, mipWidth, mipHeight, imageCount, 0, mipSize, mipData);
						}
						else
						{
							glTexImage3D(target, mip, internalFormat, mipWidth, mipHeight, imageCount, 0, format, type, mipData);
						}
					}

					GlSystem::CheckErrors("Can't set 2D array texture data");
				}
			}
			// single 2D texture
			else
			{
				target = GL_TEXTURE_2D;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 2D texture");

				if(useTextureStorage)
					glTexStorage2D(target, mips, internalFormat, imageWidth, imageHeight);

				for(int mip = 0; mip < mips; ++mip)
				{
					int linePitch = data->GetMipLinePitch(mip);
					if(pixelSize && linePitch % pixelSize)
						THROW("Wrong line pitch");
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
					glPixelStorei(GL_UNPACK_ROW_LENGTH, pixelSize ? linePitch / pixelSize : 0);
					GlSystem::CheckErrors("Can't set pixel store format");
#endif

					int mipWidth = data->GetMipWidth(mip);
					int mipHeight = data->GetMipHeight(mip);
					int mipSize = data->GetMipSize(mip);
					const void* mipData = data->GetMipData(0, mip);

					if(useTextureStorage)
					{
						if(compressed)
						{
							glCompressedTexSubImage2D(target, mip, 0, 0, mipWidth, mipHeight, internalFormat, mipSize, mipData);
						}
						else
						{
							glTexSubImage2D(target, mip, 0, 0, mipWidth, mipHeight, format, type, mipData);
						}
					}
					else
					{
						if(compressed)
						{
							glCompressedTexImage2D(target, mip, internalFormat, mipWidth, mipHeight, 0, mipSize, mipData);
						}
						else
						{
							glTexImage2D(target, mip, internalFormat, mipWidth, mipHeight, 0, format, type, mipData);
						}
					}

					GlSystem::CheckErrors("Can't set 2D texture data");
				}
			}
		}
		// 1D texture
		else
		{
			// if array
			if(imageCount)
			{
				target = GL_TEXTURE_1D_ARRAY;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 1D array texture");

				if(useTextureStorage)
					glTexStorage2D(target, mips, internalFormat, imageWidth, imageCount);

				int imagePitch = data->GetImageSize();
				if(pixelSize && imagePitch % pixelSize)
					THROW("Wrong image pitch");
				glPixelStorei(GL_UNPACK_ROW_LENGTH, pixelSize ? imagePitch / pixelSize : 0);
				for(int mip = 0; mip < mips; ++mip)
				{
					int mipWidth = data->GetMipWidth(mip);
					int mipSize = data->GetMipSize(mip);
					const void* mipData = data->GetMipData(0, mip);

					if(useTextureStorage)
					{
						if(compressed)
						{
							glCompressedTexSubImage2D(target, mip, 0, 0, mipWidth, imageCount, internalFormat, mipSize, mipData);
						}
						else
						{
							glTexSubImage2D(target, mip, 0, 0, mipWidth, imageCount, format, type, mipData);
						}
					}
					else
					{
						if(compressed)
						{
							glCompressedTexImage2D(target, mip, internalFormat, mipWidth, imageCount, 0, mipSize, mipData);
						}
						else
						{
							glTexImage2D(target, mip, internalFormat, mipWidth, imageCount, 0, format, type, mipData);
						}
					}

					GlSystem::CheckErrors("Can't set 1D array texture data");
				}
			}
			// single 1D texture
			else
			{
				target = GL_TEXTURE_1D;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 1D texture");

				if(useTextureStorage)
					glTexStorage1D(target, mips, internalFormat, imageWidth);

				for(int mip = 0; mip < mips; ++mip)
				{
					int mipWidth = data->GetMipWidth(mip);
					int mipSize = data->GetMipSize(mip);
					const void* mipData = data->GetMipData(0, mip);

					if(useTextureStorage)
					{
						if(compressed)
						{
							glCompressedTexSubImage1D(target, mip, 0, mipWidth, internalFormat, mipSize, mipData);
						}
						else
						{
#if defined(___INANITY_PLATFORM_ANDROID)
							glTexSubImage2D(target, mip, 0, 0, mipWidth, 1, format, type, mipData);
#else
							glTexSubImage1D(target, mip, 0, mipWidth, format, type, mipData);
#endif
						}
					}
					else
					{
						if(compressed)
						{
							glCompressedTexImage1D(target, mip, internalFormat, mipWidth, 0, mipSize, mipData);
						}
						else
						{
#if defined(___INANITY_PLATFORM_ANDROID)
							glTexImage2D(target, mip, internalFormat, mipWidth, 1, 0, format, type, mipData);
#else
							glTexImage1D(target, mip, internalFormat, mipWidth, 0, format, type, mipData);
#endif
						}
					}

					GlSystem::CheckErrors("Can't set 1D texture data");
				}
			}
		}

#if !defined(___INANITY_PLATFORM_EMSCRIPTEN)
		if(!useTextureStorage)
		{
			glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mips - 1);
		}
#endif

		// correct sampler settings
		SamplerSettings ss = samplerSettings;
		if(mips <= 1) ss.mipMapping = false;
#if defined(___INANITY_PLATFORM_EMSCRIPTEN)
		// disable mipmapping and set clamp wrapping mode for NPOT texture
		if((imageWidth & (imageWidth - 1)) || (imageHeight & (imageHeight - 1)))
		{
			ss.SetWrap(SamplerSettings::wrapClamp);
			ss.mipMapping = false;
		}
#endif

		SetupTextureSampling(target, ss);

		return NEW(GlTexture(internalTexture, imageWidth, imageHeight, imageDepth));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create static texture", exception);
	}
}

ptr<SamplerState> GlDevice::CreateSamplerState(const SamplerSettings& samplerSettings)
{
	BEGIN_TRY();

	GLuint samplerName;
	if(!(internalCaps & InternalCaps::samplerObjects))
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
		// HACK: go further quietly
		return 0;
#else
		THROW("Samplers are not supported");
#endif

	glGenSamplers(1, &samplerName);
	GlSystem::CheckErrors("Can't gen sampler");

	ptr<SamplerState> samplerState = NEW(GlSamplerState(this, samplerName));

	// setup sampler

	GLint minFilter, magFilter;
	ConvertSamplerSettingsFilters(samplerSettings, minFilter, magFilter);
	// set min filter
	glSamplerParameteri(samplerName, GL_TEXTURE_MIN_FILTER, minFilter);
	GlSystem::CheckErrors("Can't set sampler min filter");
	// set mag filter
	glSamplerParameteri(samplerName, GL_TEXTURE_MAG_FILTER, magFilter);
	GlSystem::CheckErrors("Can't set sampler mag filter");

	// set wrapping modes
	glSamplerParameteri(samplerName, GL_TEXTURE_WRAP_S, ConvertSamplerSettingsWrap(samplerSettings.wrapU));
	GlSystem::CheckErrors("Can't set sampler wrap U");
	glSamplerParameteri(samplerName, GL_TEXTURE_WRAP_T, ConvertSamplerSettingsWrap(samplerSettings.wrapV));
	GlSystem::CheckErrors("Can't set sampler wrap V");
	glSamplerParameteri(samplerName, GL_TEXTURE_WRAP_R, ConvertSamplerSettingsWrap(samplerSettings.wrapW));
	GlSystem::CheckErrors("Can't set sampler wrap W");

	// set min and max LOD.
	glSamplerParameterf(samplerName, GL_TEXTURE_MIN_LOD, samplerSettings.minLOD);
	GlSystem::CheckErrors("Can't set min LOD");
	glSamplerParameterf(samplerName, GL_TEXTURE_MAX_LOD, samplerSettings.maxLOD);
	GlSystem::CheckErrors("Can't set max LOD");

	// set border color
	glSamplerParameterfv(samplerName, GL_TEXTURE_BORDER_COLOR, samplerSettings.borderColor.t);
	GlSystem::CheckErrors("Can't set border color");

	return samplerState;

	END_TRY("Can't create OpenGL sampler state");
}

ptr<DepthStencilState> GlDevice::CreateDepthStencilState()
{
	return NEW(GlDepthStencilState());
}

ptr<BlendState> GlDevice::CreateBlendState()
{
	return NEW(GlBlendState());
}

END_INANITY_GRAPHICS
