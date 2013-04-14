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
#include "GlGeometry.hpp"
#include "Layout.hpp"
#include "GlInternalTexture.hpp"
#include "Image2DData.hpp"
#include "GlSamplerState.hpp"
#include "GlBlendState.hpp"
#include "GlInternalAttributeBinding.hpp"
#include "GlInternalAttributeBindingCache.hpp"
#include "GlslSource.hpp"
#include "GlShaderBindings.hpp"
#include "Win32Output.hpp"
#include "../File.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"
#ifdef ___INANITY_WINDOWS
#include "../Strings.hpp"
#endif

BEGIN_INANITY_GRAPHICS

#ifdef ___INANITY_WINDOWS
GlDevice::GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context)
: system(system), deviceName(deviceName), context(context), hglrc(0)
{
	attributeBindingCache = NEW(GlInternalAttributeBindingCache(this));
}

GlDevice::~GlDevice()
{
	if(hglrc)
		wglDeleteContext(hglrc);
}
#endif

ptr<GlInternalAttributeBinding> GlDevice::CreateInternalAttributeBinding(Layout* vertexLayout)
{
	try
	{
		const std::vector<Layout::Element>& layoutElements = vertexLayout->GetElements();
		if(layoutElements.empty())
			THROW_PRIMARY_EXCEPTION("Vertex layout is empty");

		ptr<GlInternalAttributeBinding> binding = NEW(GlInternalAttributeBinding());
		std::vector<GlInternalAttributeBinding::Element>& resultElements = binding->GetElements();

		for(size_t i = 0; i < layoutElements.size(); ++i)
		{
			const Layout::Element& layoutElement = layoutElements[i];

			// получить количество необходимых результирующих элементов, и размер элемента в байтах
			int needResultElementsCount;
			switch(layoutElement.dataType)
			{
			case DataTypes::Float4x4:
				needResultElementsCount = 4;
				break;
			default:
				needResultElementsCount = 1;
				break;
			}
			// выбрать формат и размер
			GLint size;
			GLenum type;
			switch(layoutElement.dataType)
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
			case DataTypes::Float4x4:
				size = 4;
				type = GL_FLOAT;
				break;
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
				THROW_PRIMARY_EXCEPTION("Unknown element type");
			}

			// размер элемента - пока всегда один вектор (float4 или uint4),
			// так как он нужен только для матриц float4x4
			const int elementSize = 16;

			// цикл по результирующим элементам
			for(int resultElementIndex = 0; resultElementIndex < needResultElementsCount; ++resultElementIndex)
			{
				resultElements.push_back(GlInternalAttributeBinding::Element());
				GlInternalAttributeBinding::Element& resultElement = resultElements.back();

				resultElement.index = layoutElement.semantic + resultElementIndex;
				resultElement.size = size;
				resultElement.type = type;
				resultElement.normalized = false;
				resultElement.pointer = (GLvoid*)((char*)0 + layoutElement.offset + resultElementIndex * elementSize);
			}
		}

		return binding;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create GL internal input layout", exception);
	}
}

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

		// создать временный контекст
		HGLRC hglrcTemp = wglCreateContext(hdc);
		// сделать его текущим
		if(!wglMakeCurrent(hdc, hglrcTemp))
			THROW_SECONDARY_EXCEPTION("Can't make temp OpenGL context current", Exception::SystemError());

		// создать настоящий контекст
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_FLAGS_ARB, 0
#ifdef _DEBUG
				| WGL_CONTEXT_DEBUG_BIT_ARB
#endif
			,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0, 0
		};
		PFNWGLCREATECONTEXTATTRIBSARBPROC p = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if(!p)
			THROW_PRIMARY_EXCEPTION("ppp");
		hglrc = p(hdc, 0, attribs);
		if(!hglrc)
			THROW_PRIMARY_EXCEPTION("Can't create OpenGL window context");

		// сделать его текущим
		if(!wglMakeCurrent(hdc, hglrc))
			THROW_SECONDARY_EXCEPTION("Can't make OpenGL context current", Exception::SystemError());

		// удалить временный контекст
		wglDeleteContext(hglrcTemp);

		// инициализировать GLEW ещё раз
		GlSystem::InitGLEW();

		GlSystem::ClearErrors();

		// установить размер окна
		SetWindowPos(output->GetHWND(), NULL, 0, 0, mode.width, mode.height, SWP_NOMOVE | SWP_NOZORDER);

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

ptr<Geometry> GlDevice::CreateGeometry(ptr<VertexBuffer> abstractVertexBuffer, ptr<IndexBuffer> abstractIndexBuffer)
{
	try
	{
		ptr<GlVertexBuffer> vertexBuffer = abstractVertexBuffer.FastCast<GlVertexBuffer>();
		ptr<GlIndexBuffer> indexBuffer = abstractIndexBuffer.FastCast<GlIndexBuffer>();

		// получить привязку к разметке вершинного буфера
		ptr<Layout> vertexLayout = vertexBuffer->GetLayout();
		ptr<GlInternalAttributeBinding> attributeBinding = attributeBindingCache->GetBinding(vertexLayout);

		// создать Vertex Array Object
		GLuint vertexArrayName;
		glGenVertexArrays(1, &vertexArrayName);
		GlSystem::CheckErrors("Can't gen vertex array");

		// сразу создать объект геометрии
		ptr<GlGeometry> geometry = NEW(GlGeometry(vertexBuffer, indexBuffer, vertexArrayName));

		// задать все настройки для VAO

		// привязать VAO
		glBindVertexArray(vertexArrayName);
		GlSystem::CheckErrors("Can't bind vertex array");

		// применить вершинный буфер (ссылка на который запомнится для элементов)
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetName());
		GlSystem::CheckErrors("Can't bind array buffer");
		// задать элементы в VAO
		int stride = vertexLayout->GetStride();
		const std::vector<GlInternalAttributeBinding::Element>& elements = attributeBinding->GetElements();
		for(size_t i = 0; i < elements.size(); ++i)
		{
			const GlInternalAttributeBinding::Element& element = elements[i];

			glEnableVertexAttribArray(element.index);
			GlSystem::CheckErrors("Can't enable vertex attribute array");
			glVertexAttribPointer(element.index, element.size, element.type, element.normalized, stride, element.pointer);
			GlSystem::CheckErrors("Can't bind vertex attribute");
		}
		// отвязать вершинный буфер
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		GlSystem::CheckErrors("Can't unbind array buffer");

		// привязать индексный буфер (ссылка на который будет запомнена в VAO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer ? indexBuffer->GetName() : 0);
		GlSystem::CheckErrors("Can't bind element array buffer");

		// отвязать VAO
		glBindVertexArray(0);
		GlSystem::CheckErrors("Can't unbind vertex array");

		return geometry;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create geometry", exception);
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
