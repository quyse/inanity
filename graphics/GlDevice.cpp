#include "GlDevice.hpp"
#include "GlSystem.hpp"
#include "GlPresenter.hpp"
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
#include "Image2DData.hpp"
#include "GlSamplerState.hpp"
#include "GlBlendState.hpp"
#include "GlslSource.hpp"
#include "GlShaderBindings.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"
#include "../Strings.hpp"
#ifdef ___INANITY_WINDOWS
#include "Win32Output.hpp"
#endif
#ifdef ___INANITY_LINUX
#include "X11Output.hpp"
#include "../X11Window.hpp"
#include "../X11Display.hpp"
#endif

BEGIN_INANITY_GRAPHICS

#ifdef ___INANITY_WINDOWS
GlDevice::GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context)
: system(system), deviceName(deviceName), context(context), hglrc(0) {}

GlDevice::~GlDevice()
{
	if(hglrc)
		wglDeleteContext(hglrc);
}
#endif

#ifdef ___INANITY_LINUX
GlDevice::GlDevice(ptr<GlSystem> system, ptr<GlContext> context)
: system(system), context(context) {}
#endif

ptr<System> GlDevice::GetSystem() const
{
	return system;
}

ptr<Presenter> GlDevice::CreatePresenter(ptr<Output> abstractOutput, const PresentMode& mode)
{
	try
	{

#ifdef ___INANITY_WINDOWS
		// область вывода - только Win32
		ptr<Win32Output> output = abstractOutput.DynamicCast<Win32Output>();
		if(!output)
			THROW_PRIMARY_EXCEPTION("Only Win32 output allowed");

		// если режим полноэкранный, переключить его
		if(mode.fullscreen)
		{
			// заполнить структуру режима экрана
			DEVMODE devMode;
			ZeroMemory(&devMode, sizeof(devMode));
			devMode.dmSize = sizeof(devMode);
			devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			devMode.dmBitsPerPel = 32;
			devMode.dmPelsWidth = mode.width;
			devMode.dmPelsHeight = mode.height;
			// сменить режим экрана
			if(ChangeDisplaySettingsEx(Strings::UTF82Unicode(deviceName).c_str(), &devMode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
				THROW_PRIMARY_EXCEPTION("Can't change display settings");
		}

		// получить дескриптор окна
		HDC hdc = GetDC(output->GetHWND());
		if(!hdc)
			THROW_PRIMARY_EXCEPTION("Can't get hdc");

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
			THROW_PRIMARY_EXCEPTION("Can't choose pixel format");
		// и установить этот формат
		if(!SetPixelFormat(hdc, pixelFormat, &pfd))
			THROW_PRIMARY_EXCEPTION("Can't set pixel format");

		// создать контекст
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, 0
#ifdef _DEBUG
				| WGL_CONTEXT_DEBUG_BIT_ARB
#endif
			,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0, 0
		};
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if(!wglCreateContextAttribsARB)
			THROW_PRIMARY_EXCEPTION("Can't get wglCreateContextAttribsARB");
		hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
		if(!hglrc)
			THROW_PRIMARY_EXCEPTION("Can't create OpenGL window context");

		// сделать его текущим
		if(!wglMakeCurrent(hdc, hglrc))
			THROW_SECONDARY_EXCEPTION("Can't make OpenGL context current", Exception::SystemError());

		// инициализировать GLEW
		GlSystem::InitGLEW();

		// очистка ошибок - обход бага GLEW, который может оставлять ошибки
		// (тем не менее, GLEW инициализируется нормально)
		GlSystem::ClearErrors();

		// установить размер окна
		SetWindowPos(output->GetHWND(), NULL, 0, 0, mode.width, mode.height, SWP_NOMOVE | SWP_NOZORDER);

		// создать и вернуть Presenter
		return NEW(GlPresenter(this, hdc, NEW(GlRenderBuffer(0, 0))));
#endif

#ifdef ___INANITY_LINUX
		// область вывода - только X11
		ptr<X11Output> output = abstractOutput.DynamicCast<X11Output>();
		if(!output)
			THROW_PRIMARY_EXCEPTION("Only X11 output is allowed");

		// получить окно
		ptr<X11Window> window = output->GetWindow();

		::Display* d = window->GetDisplay()->GetDisplay();

		// создать контекст
		glxContext = glXCreateContext(d, window->GetXVisualInfo(), NULL, True);
		if(!glxContext)
			THROW_PRIMARY_EXCEPTION("Can't create GLX context");

		// сделать его текущим
		glXMakeCurrent(d, window->GetHandle(), glxContext);

		// инициализировать GLEW
		GlSystem::InitGLEW();

		GlSystem::ClearErrors();

		// установить размер окна
		XResizeWindow(d, window->GetHandle(), mode.width, mode.height);

		// отобразить окно
		XMapWindow(d, window->GetHandle());

		// создать и вернуть Presenter
		return NEW(GlPresenter(this, NEW(GlRenderBuffer(0, 0))));
#endif

	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create presenter for GL device", exception);
	}
}

ptr<Context> GlDevice::GetContext()
{
	return context;
}

ptr<RenderBuffer> GlDevice::CreateRenderBuffer(int width, int height, PixelFormat pixelFormat)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(pixelFormat, internalFormat, format, type))
			THROW_PRIMARY_EXCEPTION("Invalid pixel format");
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)width, (GLsizei)height, 0, format, type, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlRenderBuffer(internalTexture, NEW(GlTexture(internalTexture))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create render buffer", exception);
	}
}

ptr<DepthStencilBuffer> GlDevice::CreateDepthStencilBuffer(int width, int height, bool canBeResource)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen textures");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlDepthStencilBuffer(internalTexture, NEW(GlTexture(internalTexture)), width, height));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create depth stencil buffer", exception);
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
			THROW_PRIMARY_EXCEPTION("Can't compile shader:\n" + Strings::File2String(code) + "\n" + log);
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

		return NEW(GlVertexShader(shaderName, shaderBindings));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create vertex shader", exception);
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

		return NEW(GlPixelShader(shaderName, shaderBindings));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create pixel shader", exception);
	}
}

ptr<UniformBuffer> GlDevice::CreateUniformBuffer(int size)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlUniformBuffer> uniformBuffer = NEW(GlUniformBuffer(bufferName, size));

		glBindBuffer(GL_UNIFORM_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return uniformBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create uniform buffer", exception);
	}
}

ptr<VertexBuffer> GlDevice::CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlVertexBuffer> vertexBuffer = NEW(GlVertexBuffer(bufferName, file->GetSize() / layout->GetStride(), layout));

		glBindBuffer(GL_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ARRAY_BUFFER, file->GetSize(), file->GetData(), GL_STATIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return vertexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create vertex buffer", exception);
	}
}

ptr<IndexBuffer> GlDevice::CreateStaticIndexBuffer(ptr<File> file, int indexSize)
{
	try
	{
		GLuint bufferName;
		glGenBuffers(1, &bufferName);
		GlSystem::CheckErrors("Can't gen buffer");
		ptr<GlIndexBuffer> indexBuffer = NEW(GlIndexBuffer(bufferName, file->GetSize() / indexSize, indexSize));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName);
		GlSystem::CheckErrors("Can't bind buffer");

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, file->GetSize(), file->GetData(), GL_STATIC_DRAW);
		GlSystem::CheckErrors("Can't setup buffer data");

		return indexBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create index buffer", exception);
	}
}

ptr<AttributeBinding> GlDevice::CreateAttributeBinding(ptr<AttributeLayout> layout)
{
	try
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
			const AttributeLayout::Slot& slot = slots[i];

			glEnableVertexAttribArray((GLuint)i);
			GlSystem::CheckErrors("Can't enable vertex attribute array");

			// выбрать формат и размер
			GLint size;
			GLenum type;
			switch(element.dataType)
			{
			case DataTypes::Float:
				size = 1;
				type = GL_FLOAT;
				break;
			case DataTypes::Float2:
				size = 2;
				type = GL_FLOAT;
				break;
			case DataTypes::Float3:
				size = 3;
				type = GL_FLOAT;
				break;
			case DataTypes::Float4:
				size = 4;
				type = GL_FLOAT;
				break;
			case DataTypes::Float4x4:
				THROW_PRIMARY_EXCEPTION("Matrices can't be used in attributes");
			case DataTypes::UInt:
				size = 1;
				type = GL_UNSIGNED_INT;
				break;
			case DataTypes::UInt2:
				size = 2;
				type = GL_UNSIGNED_INT;
				break;
			case DataTypes::UInt3:
				size = 3;
				type = GL_UNSIGNED_INT;
				break;
			case DataTypes::UInt4:
				size = 4;
				type = GL_UNSIGNED_INT;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Unknown attribute element type");
			}
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
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create GL attribute binding", exception);
	}
}

ptr<Texture> GlDevice::CreateStaticTexture(ptr<File> file)
{
	THROW_PRIMARY_EXCEPTION("This method is unsupported on GlDevice");
}

ptr<Texture> GlDevice::CreateStatic2DTexture(ptr<Image2DData> imageData)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		GLint internalFormat;
		GLenum format;
		GLenum type;
		if(!GlSystem::GetTextureFormat(imageData->GetPixelFormat(), internalFormat, format, type))
			THROW_PRIMARY_EXCEPTION("Invalid pixel format");
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)imageData->GetWidth(), (GLsizei)imageData->GetHeight(), 0, format, type, imageData->GetData()->GetData());
		GlSystem::CheckErrors("Can't initialize texture");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		GlSystem::CheckErrors("Can't set texture parameters");

		return NEW(GlTexture(internalTexture));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create static texture", exception);
	}
}

ptr<SamplerState> GlDevice::CreateSamplerState()
{
	try
	{
		GLuint samplerName;
		glGenSamplers(1, &samplerName);
		GlSystem::CheckErrors("Can't gen sampler");

		return NEW(GlSamplerState(samplerName));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create OpenGL sampler state", exception);
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
		THROW_SECONDARY_EXCEPTION("Can't create OpenGL blend state", exception);
	}
}

END_INANITY_GRAPHICS
