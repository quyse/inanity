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
#include "Layout.hpp"
#include "GlInternalTexture.hpp"
#include "Win32Output.hpp"
#include "../File.hpp"
#include "../Exception.hpp"
#ifdef ___INANITY_WINDOWS
#include "../Strings.hpp"
#endif

#ifdef ___INANITY_WINDOWS
GlDevice::GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context)
: system(system), deviceName(deviceName), context(context), hglrc(0)
{
}

GlDevice::~GlDevice()
{
	if(hglrc)
		wglDeleteContext(hglrc);
}
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
		hglrc = wglCreateContext(hdc);
		if(!hglrc)
			THROW_PRIMARY_EXCEPTION("Can't create OpenGL window context");

		// сделать его текущим
		if(!wglMakeCurrent(hdc, hglrc))
			THROW_SECONDARY_EXCEPTION("Can't make OpenGL context current", Exception::SystemError());

		// время для инициализации GLEW
		GlSystem::InitGLEW();

		// создать и вернуть Presenter
		return NEW(GlPresenter(this, hdc, NEW(GlRenderBuffer(0, 0))));

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

		return NEW(GlDepthStencilBuffer(internalTexture, NEW(GlTexture(internalTexture))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create depth stencil buffer", exception);
	}
}

void GlDevice::CompileShader(GLuint shaderName, ptr<File> file)
{
	// указать текст шейдера
	const GLchar* string = (const GLchar*)file->GetData();
	const GLint length = (GLint)file->GetSize();
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
			THROW_PRIMARY_EXCEPTION("Can't compile shader:\n" + log);
		}
	}
}

ptr<VertexShader> GlDevice::CreateVertexShader(ptr<File> file)
{
	try
	{
		GLuint shaderName = glCreateShader(GL_VERTEX_SHADER);
		GlSystem::CheckErrors("Can't create shader");

		CompileShader(shaderName, file);

		return NEW(GlVertexShader(shaderName));
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

		CompileShader(shaderName, file);

		return NEW(GlPixelShader(shaderName));
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

ptr<VertexBuffer> GlDevice::CreateVertexBuffer(ptr<File> file, ptr<Layout> layout)
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

ptr<IndexBuffer> GlDevice::CreateIndexBuffer(ptr<File> file, int indexSize)
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

ptr<Texture> GlDevice::CreateStaticTexture(ptr<File> file)
{
	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

#error Work is not finished...
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
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create static texture", exception);
	}
}
