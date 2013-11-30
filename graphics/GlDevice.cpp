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
#include "GlBlendState.hpp"
#include "GlslSource.hpp"
#include "GlShaderBindings.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"
#include "../Strings.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "WglPresenter.hpp"
#include "Win32Output.hpp"
#include "Win32MonitorMode.hpp"
#elif defined(___INANITY_PLATFORM_LINUX)
#include "GlxPresenter.hpp"
#include "X11Output.hpp"
#include "../platform/X11Window.hpp"
#include "../platform/X11Display.hpp"
#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)
#include "EmsPresenter.hpp"
#include "EmsOutput.hpp"
#include <emscripten/emscripten.h>
#else
#error Unknown platform
#endif

BEGIN_INANITY_GRAPHICS

#if defined(___INANITY_PLATFORM_WINDOWS)

GlDevice::GlDevice(ptr<GlSystem> system, const String& deviceName)
: system(system), deviceName(deviceName), hglrc(0) {}

GlDevice::~GlDevice()
{
	if(hglrc)
		wglDeleteContext(hglrc);
}

#elif defined(___INANITY_PLATFORM_LINUX)

GlDevice::GlDevice(ptr<GlSystem> system)
: system(system), glxContext(0) {}

GlDevice::~GlDevice()
{
	if(display && glxContext)
	{
		glXMakeCurrent(display->GetDisplay(), None, 0);
		glXDestroyContext(display->GetDisplay(), glxContext);
	}
}

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

GlDevice::GlDevice(ptr<GlSystem> system)
: system(system) {}

GlDevice::~GlDevice() {}

#else

#error Unknown platform

#endif

void GlDevice::InitCaps()
{
#if defined(___INANITY_PLATFORM_WINDOWS) || defined(___INANITY_PLATFORM_LINUX)
	internalCaps =
		(GLEW_ARB_uniform_buffer_object ? InternalCaps::uniformBufferObject : 0) |
		(GLEW_ARB_sampler_objects ? InternalCaps::samplerObjects : 0) |
		(GLEW_ARB_vertex_attrib_binding ? InternalCaps::vertexAttribBinding : 0) |
		(GLEW_ARB_framebuffer_object ? InternalCaps::frameBufferObject : 0);
		;
	caps.flags =
		(GLEW_ARB_instanced_arrays ? Caps::attributeInstancing : 0) |
		(GLEW_ARB_draw_instanced ? Caps::drawInstancing : 0);
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

ptr<System> GlDevice::GetSystem() const
{
	return system;
}

ptr<Presenter> GlDevice::CreatePresenter(ptr<Output> abstractOutput, ptr<MonitorMode> abstractMode)
{
	try
	{

#if defined(___INANITY_PLATFORM_WINDOWS)

		// область вывода - только Win32
		ptr<Win32Output> output = abstractOutput.DynamicCast<Win32Output>();
		if(!output)
			THROW("Only Win32 output allowed");
		// режим экрана - только Win32
		ptr<Win32MonitorMode> mode = abstractMode.DynamicCast<Win32MonitorMode>();
		if(!mode && abstractMode)
			THROW("Only Win32 monitor mode allowed");

		// если режим полноэкранный, переключить его
		if(mode)
		{
			// получить монитор окна
			HMONITOR monitor = MonitorFromWindow(output->GetHWND(), MONITOR_DEFAULTTOPRIMARY);
			if(!monitor)
				THROW("Can't get window monitor");
			MONITORINFOEX monitorInfo;
			monitorInfo.cbSize = sizeof(monitorInfo);
			if(!GetMonitorInfo(monitor, &monitorInfo))
				THROW("Can't get monitor info");
			DEVMODE modeInfo = mode->GetInfo();
			if(ChangeDisplaySettingsEx(monitorInfo.szDevice, &modeInfo, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
				THROW("Can't change display settings");
		}

		// получить дескриптор окна
		HDC hdc = GetDC(output->GetHWND());
		if(!hdc)
			THROW("Can't get hdc");

		// подобрать формат пикселов
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 0; // не используем глубину для первичного буфера
		pfd.iLayerType = PFD_MAIN_PLANE;
		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if(!pixelFormat)
			THROW("Can't choose pixel format");
		// и установить этот формат
		if(!SetPixelFormat(hdc, pixelFormat, &pfd))
			THROW("Can't set pixel format");

		// создать временный контекст
		HGLRC hglrcTemp = wglCreateContext(hdc);
		// сделать его текущим
		if(!wglMakeCurrent(hdc, hglrcTemp))
			THROW_SECONDARY("Can't make temp OpenGL context current", Exception::SystemError());

		// создать настоящий контекст
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
		// версии для попыток создания
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
		// цикл по попыткам создать контекст для какой-нибудь версии
		for(int i = 0; i < sizeof(versions) / sizeof(versions[0]); ++i)
		{
			attribs[1] = versions[i][0];
			attribs[3] = versions[i][1];
			hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
			if(hglrc)
				break;
		}
		// если никакая версия OpenGL не поддерживается, то жопа
		if(!hglrc)
			THROW("Can't create OpenGL window context");

		// сделать новый контекст текущим
		if(!wglMakeCurrent(hdc, hglrc))
			THROW_SECONDARY("Can't make OpenGL context current", Exception::SystemError());

		// удалить временный контекст
		wglDeleteContext(hglrcTemp);

		// инициализировать GLEW
		GlSystem::InitGLEW();

		// init capabilities
		InitCaps();

		// очистка ошибок - обход бага GLEW, который может оставлять ошибки
		// (тем не менее, GLEW инициализируется нормально)
		GlSystem::ClearErrors();

		// создать и вернуть Presenter
		return NEW(WglPresenter(this, NEW(GlFrameBuffer(this, 0)), hdc));

#elif defined(___INANITY_PLATFORM_LINUX)

		// only X11Output allowed
		ptr<X11Output> output = abstractOutput.DynamicCast<X11Output>();
		if(!output)
			THROW("Only X11 output is allowed");

		ptr<Platform::X11Window> window = output->GetWindow();

		display = window->GetDisplay();

		::Display* xDisplay = display->GetDisplay();

		static const int fbAttribs[] =
		{
			GLX_X_RENDERABLE, True,
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
			GLX_RENDER_TYPE, GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
			GLX_RED_SIZE, 8,
			GLX_GREEN_SIZE, 8,
			GLX_BLUE_SIZE, 8,
			GLX_ALPHA_SIZE, 8,
			GLX_DEPTH_SIZE, 24,
			GLX_STENCIL_SIZE, 8,
			GLX_DOUBLEBUFFER, True,
			0, 0
		};

#define GETPROC(fnType, fn) \
		fnType fn = (fnType)glXGetProcAddress((const GLubyte*)#fn); \
		if(!fn) THROW("Can't get " #fn)

		GETPROC(PFNGLXCHOOSEFBCONFIGPROC, glXChooseFBConfig);
		GETPROC(PFNGLXGETFBCONFIGATTRIBPROC, glXGetFBConfigAttrib);
		GETPROC(PFNGLXGETVISUALFROMFBCONFIGPROC, glXGetVisualFromFBConfig);

		// get FB configs
		int fbConfigsCount;
		GLXFBConfig* fbConfigs = glXChooseFBConfig(
			xDisplay,
			DefaultScreen(xDisplay),
			fbAttribs,
			&fbConfigsCount
		);
		if(!fbConfigs || !fbConfigsCount)
			THROW("Can't get FB configs");

		// select first config
		GLXFBConfig fbConfig = fbConfigs[0];
		XFree(fbConfigs);
		int visualId;
		if(glXGetFBConfigAttrib(xDisplay, fbConfig, GLX_VISUAL_ID, &visualId))
			THROW("Can't get visual id of FB config");
		XVisualInfo* xVisualInfo = glXGetVisualFromFBConfig(xDisplay, fbConfig);
		if(!xVisualInfo)
			THROW("Can't get visual from FB config");

		// create temp old-style context
		GLXContext tempGlxContext = glXCreateContext(xDisplay, xVisualInfo, 0, True);
		if(!tempGlxContext)
			THROW("Can't create temp context");
		// make it current
		glXMakeCurrent(xDisplay, window->GetHandle(), tempGlxContext);

		GETPROC(PFNGLXCREATECONTEXTATTRIBSARBPROC, glXCreateContextAttribsARB);

		// reset and delete temp context
		glXMakeCurrent(xDisplay, None, 0);
		glXDestroyContext(xDisplay, tempGlxContext);

		// now create real context

		// attributes for context
		int attribs[] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 0,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			GLX_CONTEXT_FLAGS_ARB, 0
#ifdef _DEBUG
				| GLX_CONTEXT_DEBUG_BIT_ARB
#endif
			,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			0, 0
		};
		// versions for tryings
		static const int versions[][2] =
		{
			{ 4, 3 },
			{ 4, 2 },
			{ 4, 1 },
			{ 4, 0 },
			{ 3, 3 },
			{ 3, 2 }
		};
		// loop trying to create some version
		for(int i = 0; i < sizeof(versions) / sizeof(versions[0]); ++i)
		{
			attribs[1] = versions[i][0];
			attribs[3] = versions[i][1];
			glxContext = glXCreateContextAttribsARB(xDisplay, fbConfig, 0, True, attribs);
			if(glxContext)
				break;
		}

		// if no version of OpenGL supported, this is end
		if(!glxContext)
			THROW("Can't create OpenGL context");

		GETPROC(PFNGLXCREATEWINDOWPROC, glXCreateWindow);
		GETPROC(PFNGLXMAKECONTEXTCURRENTPROC, glXMakeContextCurrent);

		// create glx window
		GLXWindow glxWindow = glXCreateWindow(xDisplay, fbConfig, window->GetHandle(), 0);
		if(!glxWindow)
		{
			glXDestroyContext(xDisplay, glxContext);
			THROW("Can't create GLX window");
		}

		// make OpenGL context current
		if(!glXMakeContextCurrent(xDisplay, glxWindow, glxWindow, glxContext))
		{
			glXDestroyContext(xDisplay, glxContext);
			THROW("Can't make context current");
		}

		GlSystem::InitGLEW();
		GlSystem::ClearErrors();

		// init capabilities
		InitCaps();

		return NEW(GlxPresenter(this, NEW(GlFrameBuffer(this, 0)), output, glxWindow));

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

		EM_ASM(
			window.Browser.createContext(window.Module.canvas, true, true);
		);

		ptr<EmsOutput> output = abstractOutput.DynamicCast<EmsOutput>();
		if(!output)
			THROW("Only Emscripten output allowed");

		// init capabilities
		InitCaps();

		return NEW(EmsPresenter(this, NEW(GlFrameBuffer(this, 0)), output));

#else

#error Unknown platform

#endif

	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create presenter for GL device", exception);
	}
}

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

ptr<RenderBuffer> GlDevice::CreateRenderBuffer(int width, int height, PixelFormat pixelFormat)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(this, textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(pixelFormat, internalFormat, format, type))
			THROW("Invalid pixel format");

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)width, (GLsizei)height, 0, format, type, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlRenderBuffer(internalTexture, NEW(GlTexture(internalTexture))));
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

		glTexImage2D(GL_TEXTURE_2D, 0, /*GL_DEPTH24_STENCIL8*/ GL_DEPTH_STENCIL, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlDepthStencilBuffer(internalTexture, NEW(GlTexture(internalTexture)), width, height));
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
		ptr<GlVertexBuffer> vertexBuffer = NEW(GlVertexBuffer(this, bufferName, file->GetSize() / layout->GetStride(), layout));

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

		glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
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
		ptr<GlIndexBuffer> indexBuffer = NEW(GlIndexBuffer(this, bufferName, file->GetSize() / indexSize, indexSize));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, file->GetSize(), file->GetData(), GL_STATIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return indexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create index buffer", exception);
	}
}

void GlDevice::GetAttributeSizeAndType(DataType dataType, GLint& size, GLenum& type, bool& integer)
{
	switch(dataType)
	{
	case DataTypes::_float:
		size = 1;
		type = GL_FLOAT;
		integer = false;
		break;
	case DataTypes::_vec2:
		size = 2;
		type = GL_FLOAT;
		integer = false;
		break;
	case DataTypes::_vec3:
		size = 3;
		type = GL_FLOAT;
		integer = false;
		break;
	case DataTypes::_vec4:
		size = 4;
		type = GL_FLOAT;
		integer = false;
		break;
	case DataTypes::_mat4x4:
		THROW("Matrices can't be used in attributes");
	case DataTypes::_uint:
		size = 1;
		type = GL_UNSIGNED_INT;
		integer = true;
		break;
	case DataTypes::_uvec2:
		size = 2;
		type = GL_UNSIGNED_INT;
		integer = true;
		break;
	case DataTypes::_uvec3:
		size = 3;
		type = GL_UNSIGNED_INT;
		integer = true;
		break;
	case DataTypes::_uvec4:
		size = 4;
		type = GL_UNSIGNED_INT;
		integer = true;
		break;
	default:
		THROW("Unknown attribute element type");
	}
}

ptr<AttributeBinding> GlDevice::CreateAttributeBinding(ptr<AttributeLayout> layout)
{
	try
	{
		// если поддерживается vertex bindings, используем их как более оптимальный метод
		if(internalCaps & InternalCaps::vertexAttribBinding)
		{
			// создать Vertex Array Object
			GLuint vertexArrayName;
			glGenVertexArrays(1, &vertexArrayName);
			GlSystem::CheckErrors("Can't gen vertex array");

			// сразу создать привязку
			ptr<AttributeBinding> binding = NEW(GlAttributeBinding(this, vertexArrayName));

			// задать все настройки для VAO

			// привязать VAO
			glBindVertexArray(vertexArrayName);
			GlSystem::CheckErrors("Can't bind vertex array");

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
				GetAttributeSizeAndType(element.dataType, size, type, integer);

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

			// отвязать VAO
			glBindVertexArray(0);
			GlSystem::CheckErrors("Can't unbind vertex array");

			// вернуть привязку
			return binding;
		}
		// иначе использовать "ручную" привязку
		else
		{
			// создать привязку
			ptr<GlAttributeBinding> binding = NEW(GlAttributeBinding(this, 0));

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
				GetAttributeSizeAndType(element.dataType, bindingElement.size, bindingElement.type, bindingElement.integer);
				bindingElement.normalized = false;
				bindingElement.pointer = (GLvoid*)element.offset;

				bindingSlot.elements.push_back(bindingElement);
			}

			// вернуть привязку
			return binding;
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create GL attribute binding", exception);
	}
}

ptr<Texture> GlDevice::CreateStaticTexture(ptr<RawTextureData> data)
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

		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(data->GetFormat(), internalFormat, format, type))
			THROW("Invalid pixel format");

		int pixelSize = PixelFormat::GetPixelSize(data->GetFormat().size);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLenum target;

		// 3D texture
		if(data->GetImageDepth())
		{
			// if array
			if(data->GetCount())
				THROW("Arrays of 3D textures are not supported");

			target = GL_TEXTURE_3D;

			glBindTexture(target, textureName);
			GlSystem::CheckErrors("Can't bind 3D texture");

			for(int mip = 0; mip < mips; ++mip)
			{
				int linePitch = data->GetMipLinePitch(mip);
				int slicePitch = data->GetMipSlicePitch(mip);
				if((linePitch % pixelSize) || (slicePitch % pixelSize))
					THROW("Wrong line or slice pitch");
				glPixelStorei(GL_UNPACK_ROW_LENGTH, linePitch / pixelSize);
				glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, slicePitch / pixelSize);
				glTexImage3D(target, mip, internalFormat,
					data->GetMipWidth(mip), data->GetMipHeight(mip), data->GetMipDepth(mip),
					0, format, type, data->GetMipData(0, mip));
				GlSystem::CheckErrors("Can't set 3D texture data");
			}
		}
		// 2D texture
		else if(data->GetImageHeight())
		{
			// if array
			if(data->GetCount())
			{
				target = GL_TEXTURE_2D_ARRAY;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 2D array texture");

				int imagePitch = data->GetImageSize();
				if(imagePitch % pixelSize)
					THROW("Wrong image pitch");
				glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, imagePitch / pixelSize);
				for(int mip = 0; mip < mips; ++mip)
				{
					int linePitch = data->GetMipLinePitch(mip);
					if(linePitch % pixelSize)
						THROW("Wrong line pitch");
					glPixelStorei(GL_UNPACK_ROW_LENGTH, linePitch / pixelSize);
					glTexImage3D(target, mip, internalFormat,
						data->GetMipWidth(mip), data->GetMipHeight(mip), data->GetCount(),
						0, format, type, data->GetMipData(0, mip));
					GlSystem::CheckErrors("Can't set 2D array texture data");
				}
			}
			// single 2D texture
			else
			{
				target = GL_TEXTURE_2D;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 2D texture");

				for(int mip = 0; mip < mips; ++mip)
				{
					int linePitch = data->GetMipLinePitch(mip);
					if(linePitch % pixelSize)
						THROW("Wrong line pitch");
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
					glPixelStorei(GL_UNPACK_ROW_LENGTH, linePitch / pixelSize);
					GlSystem::CheckErrors("Can't set pixel store format");
#endif
					glTexImage2D(target, mip, internalFormat,
						data->GetMipWidth(mip), data->GetMipHeight(mip),
						0, format, type, data->GetMipData(0, mip));
					GlSystem::CheckErrors("Can't set 2D texture data");
				}
			}
		}
		// 1D texture
		else
		{
			// if array
			if(data->GetCount())
			{
				target = GL_TEXTURE_1D_ARRAY;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 1D array texture");

				int imagePitch = data->GetImageSize();
				if(imagePitch % pixelSize)
					THROW("Wrong image pitch");
				glPixelStorei(GL_UNPACK_ROW_LENGTH, imagePitch / pixelSize);
				for(int mip = 0; mip < mips; ++mip)
				{
					glTexImage2D(target, mip, internalFormat,
						data->GetMipWidth(mip), data->GetCount(),
						0, format, type, data->GetMipData(0, mip));
					GlSystem::CheckErrors("Can't set 1D array texture data");
				}
			}
			// single 1D texture
			else
			{
				target = GL_TEXTURE_1D;

				glBindTexture(target, textureName);
				GlSystem::CheckErrors("Can't bind 1D texture");

				for(int mip = 0; mip < mips; ++mip)
				{
					glTexImage1D(target, mip, internalFormat,
						data->GetMipWidth(mip),
						0, format, type, data->GetMipData(0, mip));
					GlSystem::CheckErrors("Can't set 1D texture data");
				}
			}
		}

		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlTexture(internalTexture));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create static texture", exception);
	}
}

ptr<SamplerState> GlDevice::CreateSamplerState()
{
	try
	{
		GLuint samplerName;
		if(internalCaps & InternalCaps::samplerObjects)
		{
			glGenSamplers(1, &samplerName);
			GlSystem::CheckErrors("Can't gen sampler");
		}
		else
			samplerName = 0;

		return NEW(GlSamplerState(this, samplerName));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create OpenGL sampler state", exception);
	}
}

ptr<BlendState> GlDevice::CreateBlendState()
{
	try
	{
		return NEW(GlBlendState());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create OpenGL blend state", exception);
	}
}

END_INANITY_GRAPHICS
