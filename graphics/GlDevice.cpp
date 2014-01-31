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
#include "Win32MonitorMode.hpp"
#include "../platform/Win32Window.hpp"
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
#include "SdlPresenter.hpp"
#include "SdlMonitorMode.hpp"
#include "../platform/Sdl.hpp"
#include "../platform/SdlWindow.hpp"
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
: system(system), deviceName(deviceName), hglrc(0) {}

GlDevice::~GlDevice()
{
	if(hglrc)
		wglDeleteContext(hglrc);
}

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)

GlDevice::GlDevice(ptr<GlSystem> system)
: system(system), glContext(0) {}

GlDevice::~GlDevice()
{
	if(glContext)
		SDL_GL_DeleteContext(glContext);
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
#if defined(___INANITY_PLATFORM_WINDOWS) || defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
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

ptr<Presenter> GlDevice::CreateWindowPresenter(ptr<Platform::Window> abstractWindow, ptr<MonitorMode> abstractMode)
{
	BEGIN_TRY();

#if defined(___INANITY_PLATFORM_WINDOWS)

	ptr<Platform::Win32Window> window = abstractWindow.DynamicCast<Platform::Win32Window>();
	if(!window)
		THROW("Only Win32 window is allowed");
	ptr<Win32MonitorMode> mode = abstractMode.DynamicCast<Win32MonitorMode>();
	if(!mode && abstractMode)
		THROW("Only Win32 monitor mode allowed");

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)

	ptr<Platform::SdlWindow> window = abstractWindow.DynamicCast<Platform::SdlWindow>();
	if(!window)
		THROW("Only SDL window is allowed");
	ptr<SdlMonitorMode> mode = abstractMode.DynamicCast<SdlMonitorMode>();
	if(!mode && abstractMode)
		THROW("Only SDL monitor mode is allowed");

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

	ptr<Platform::EmsWindow> window = abstractWindow.DynamicCast<Platform::EmsWindow>();
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

	// если режим полноэкранный, переключить его
	if(mode)
	{
		// получить монитор окна
		HMONITOR monitor = MonitorFromWindow(window->GetHWND(), MONITOR_DEFAULTTOPRIMARY);
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
	HDC hdc = GetDC(window->GetHWND());
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
	return NEW(WglPresenter(this, NEW(GlFrameBuffer(this, 0)), hdc, window));

	END_TRY("Can't create Windows OpenGL presenter");
}

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)

ptr<SdlPresenter> GlDevice::CreatePresenter(ptr<Platform::SdlWindow> window, ptr<SdlMonitorMode> mode)
{
	BEGIN_TRY();

	glContext = SDL_GL_CreateContext(window->GetHandle());
	if(!glContext)
		THROW_SECONDARY("Can't create OpenGL context", Platform::Sdl::Error());

	GlSystem::InitGLEW();
	GlSystem::ClearErrors();

	// init capabilities
	InitCaps();

	return NEW(SdlPresenter(this, NEW(GlFrameBuffer(this, 0)), window));

	END_TRY("Can't create SDL OpenGL presenter");
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

		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(pixelFormat, true, internalFormat, format, type))
			THROW("Invalid pixel format");

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)width, (GLsizei)height, 0, format, type, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		SetupTextureSampling(GL_TEXTURE_2D, samplerSettings);

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
				GetAttributeSizeAndType(element.dataType, element.layoutDataType, bindingElement.size, bindingElement.type, bindingElement.integer);
				bindingElement.normalized = false;
				bindingElement.pointer = (GLvoid*)(size_t)element.offset;

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

		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(data->GetFormat(), false, internalFormat, format, type))
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

		SetupTextureSampling(target, samplerSettings);

		return NEW(GlTexture(internalTexture));
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
