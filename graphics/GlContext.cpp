#include "GlContext.hpp"
#include "GlSystem.hpp"
#include "GlInternalProgramCache.hpp"
#include "GlInternalProgram.hpp"
#include "GlInternalAttributeBinding.hpp"
#include "GlInternalAttributeBindingCache.hpp"
#include "GlRenderBuffer.hpp"
#include "GlDepthStencilBuffer.hpp"
#include "GlTexture.hpp"
#include "GlSampler.hpp"
#include "GlUniformBuffer.hpp"
#include "GlVertexShader.hpp"
#include "GlPixelShader.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "Layout.hpp"
#include "../Exception.hpp"

GlContext::GlContext() :
	targetsFramebuffer(0), serviceFramebuffer(0),
	currentFramebuffer(0), dirtyCurrentFramebuffer(true)
{
	programCache = NEW(GlInternalProgramCache());
}

GlContext::~GlContext()
{
	if(targetsFramebuffer)
		glDeleteFramebuffers(1, &targetsFramebuffer);
	if(serviceFramebuffer)
		glDeleteFramebuffers(1, &serviceFramebuffer);
}

void GlContext::BindServiceFramebuffer()
{
	if(!serviceFramebuffer)
		glGenFramebuffers(1, &serviceFramebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, serviceFramebuffer);

	dirtyCurrentFramebuffer = true;
}

ptr<GlInternalAttributeBinding> GlContext::CreateInternalAttributeBinding(Layout* vertexLayout, GlInternalProgram* program)
{
	try
	{
		const std::vector<Layout::Element>& layoutElements = vertexLayout->GetElements();
		if(layoutElements.empty())
			THROW_PRIMARY_EXCEPTION("Vertex layout is empty");

		const std::vector<String>& attributes = program->GetAttributes();

		GLuint programName = program->GetName();

		ptr<GlInternalAttributeBinding> binding = NEW(GlInternalAttributeBinding());
		std::vector<GlInternalAttributeBinding::Element>& resultElements = binding->GetElements();

		for(size_t i = 0; i < attributes.size(); ++i)
		{
			const String& attribute = attributes[i];

			// получить соответствующий элемент разметки вершины
			// для этого преобразовать имя атрибута в семантику
			int semantic = GlSystem::AttributeNameToSemantic(attributes[i]);
			size_t layoutElementIndex;
			for(layoutElementIndex = 0; layoutElementIndex < layoutElements.size(); ++layoutElementIndex)
				if(layoutElements[layoutElementIndex].semantic == semantic)
					break;
			if(layoutElementIndex >= layoutElements.size())
				THROW_PRIMARY_EXCEPTION("Can't find element for attribute");

			const Layout::Element& layoutElement = layoutElements[layoutElementIndex];

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

			// получить локацию атрибута
			GLuint location = glGetAttribLocation(programName, attribute.c_str());

			// цикл по результирующим элементам
			for(int resultElementIndex = 0; resultElementIndex < needResultElementsCount; ++resultElementIndex)
			{
				resultElements.push_back(GlInternalAttributeBinding::Element());
				GlInternalAttributeBinding::Element& resultElement = resultElements.back();

				resultElement.index = location + resultElementIndex;
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

void GlContext::Update()
{
	// установить фреймбуфер в то, чем он был
	// далее его могут поменять, но здесь мы гарантируем, что он
	// соответствует тому, что в currentFramebuffer
	if(dirtyCurrentFramebuffer)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer);
		dirtyCurrentFramebuffer = false;
	}

	// проверить, если первый рендербуфер - по умолчанию (а непервым он быть не может),
	// то он должен быть единственным, и depth-stencil буфера не может быть
	if(boundRenderBuffers[0] && fast_cast<GlRenderBuffer*>(&*boundRenderBuffers[0])->GetName() == 0)
	{
		if(boundDepthStencilBuffer)
			THROW_PRIMARY_EXCEPTION("Default renderbuffer can be bound only without depth-stencil buffer");
		for(int i = 1; i < renderTargetSlotsCount; ++i)
			if(boundRenderBuffers[i])
				THROW_PRIMARY_EXCEPTION("Default renderbuffer can be bound only without other buffers");

		// привязать фреймбуфер по умолчанию
		if(currentFramebuffer != 0)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer = 0);

		// больше ничего привязывать не надо, только очистить флажки
		for(int i = 0; i < renderTargetSlotsCount; ++i)
			dirtyRenderBuffers[i] = false;
	}
	// иначе все рендербуферы - не по умолчанию
	else
	{
		// привязать нужный фреймбуфер
		if(currentFramebuffer != targetsFramebuffer)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer = targetsFramebuffer);

		// привязать изменившиеся рендербуферы
		for(int i = 0; i < renderTargetSlotsCount; ++i)
			if(dirtyRenderBuffers[i])
			{
				GLuint name = fast_cast<GlRenderBuffer*>(&*boundRenderBuffers[i])->GetName();
				if(name == 0)
					THROW_PRIMARY_EXCEPTION("Default renderbuffer can't be bound with other buffers");
				glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, name, 0);
				dirtyRenderBuffers[i] = false;
			}

		// привязать изменившийся depth-stencil буфер
		if(dirtyDepthStencilBuffer)
		{
			glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, fast_cast<GlDepthStencilBuffer*>(&*boundDepthStencilBuffer)->GetName(), 0);

			dirtyDepthStencilBuffer = false;
		}
	}

	// текстуры
	for(int i = 0; i < textureSlotsCount; ++i)
		if(dirtyTextures[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, fast_cast<GlTexture*>(&*boundTextures[i])->GetName());

			dirtyTextures[i] = false;
		}
	// семплеры
	for(int i = 0; i < textureSlotsCount; ++i)
		if(dirtySamplers[i])
		{
			glBindSampler(i, fast_cast<GlSampler*>(&*boundSamplers[i])->GetName());

			dirtySamplers[i] = false;
		}

	// uniform-буферы
	for(int i = 0; i < uniformBufferSlotsCount; ++i)
		if(dirtyUniformBuffers[i])
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, i, fast_cast<GlUniformBuffer*>(&*boundUniformBuffers[i])->GetName());

			dirtyUniformBuffers[i] = false;
		}

	bool dirtyAttributeBinding = false;

	// вершинный и пиксельный шейдеры
	if(dirtyVertexShader || dirtyPixelShader)
	{
		ptr<GlInternalProgram> program = programCache->GetProgram(fast_cast<GlVertexShader*>(&*boundVertexShader), fast_cast<GlPixelShader*>(&*boundPixelShader));
		if(boundProgram != program)
		{
			boundProgram = program;
			glUseProgram(boundProgram->GetName());
		}

		dirtyVertexShader = false;
		dirtyPixelShader = false;
		dirtyAttributeBinding = true;
	}

	// вершинный буфер
	if(dirtyVertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, fast_cast<GlVertexBuffer*>(&*boundVertexBuffer)->GetName());

		dirtyVertexBuffer = false;
		dirtyAttributeBinding = true;
	}

	// индексный буфер
	if(dirtyIndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundIndexBuffer ? fast_cast<GlVertexBuffer*>(&*boundIndexBuffer)->GetName() : 0);

		dirtyIndexBuffer = false;
		dirtyAttributeBinding = true;
	}

	// привязка к атрибутам
	if(dirtyAttributeBinding)
	{
		ptr<Layout> vertexLayout = boundVertexBuffer->GetLayout();
		ptr<GlInternalAttributeBinding> attributeBinding = attributeBindingCache->GetBinding(vertexLayout, boundProgram);
		if(boundAttributeBinding != attributeBinding)
		{
			boundAttributeBinding = attributeBinding;

			// выполнить привязку
			int stride = vertexLayout->GetStride();
			const std::vector<GlInternalAttributeBinding::Element>& elements = boundAttributeBinding->GetElements();
			for(size_t i = 0; i < elements.size(); ++i)
			{
				const GlInternalAttributeBinding::Element& element = elements[i];

				glVertexAttribPointer(element.index, element.size, element.type, element.normalized, stride, element.pointer);
			}
		}

		dirtyAttributeBinding = false;
	}

	if(dirtyFillMode)
	{
		switch(fillMode)
		{
		case fillModeWireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case fillModeSolid:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}

		dirtyFillMode = false;
	}

	if(dirtyCullMode)
	{
		switch(cullMode)
		{
		case cullModeNone:
			glDisable(GL_CULL_FACE);
			break;
		case cullModeBack:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case cullModeFront:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		}

		dirtyCullMode = false;
	}

	if(dirtyViewport)
	{
		glViewport(0, 0, viewportWidth, viewportHeight);

		dirtyViewport = false;
	}

	if(dirtyDepthTestFunc)
	{
		GLenum func = GL_NEVER;
		switch(depthTestFunc)
		{
		case depthTestFuncNever:
			func = GL_NEVER;
			break;
		case depthTestFuncLess:
			func = GL_LESS;
			break;
		case depthTestFuncLessOrEqual:
			func = GL_LEQUAL;
			break;
		case depthTestFuncEqual:
			func = GL_EQUAL;
			break;
		case depthTestFuncNonEqual:
			func = GL_NOTEQUAL;
			break;
		case depthTestFuncGreaterOrEqual:
			func = GL_GEQUAL;
			break;
		case depthTestFuncGreater:
			func = GL_GREATER;
			break;
		case depthTestFuncAlways:
			func = GL_ALWAYS;
			break;
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(func);

		dirtyDepthTestFunc = false;
	}

	if(dirtyDepthWrite)
	{
		glDepthMask(depthWrite ? GL_TRUE : GL_FALSE);

		dirtyDepthWrite = false;
	}
}

void GlContext::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	BindServiceFramebuffer();
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fast_cast<GlRenderBuffer*>(renderBuffer)->GetName(), 0);
	glClearBufferfv(GL_COLOR, 0, color);
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth)
{
	BindServiceFramebuffer();
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fast_cast<GlDepthStencilBuffer*>(depthStencilBuffer)->GetName(), 0);
	glClearBufferfv(GL_DEPTH, 0, &depth);
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil)
{
	BindServiceFramebuffer();
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, fast_cast<GlDepthStencilBuffer*>(depthStencilBuffer)->GetName(), 0);
	GLint s = stencil;
	glClearBufferiv(GL_STENCIL, 0, &s);
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil)
{
	BindServiceFramebuffer();
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, fast_cast<GlDepthStencilBuffer*>(depthStencilBuffer)->GetName(), 0);
	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
}

void GlContext::SetUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, size_t size)
{
	GlUniformBuffer* uniformBuffer = fast_cast<GlUniformBuffer*>(&*abstractUniformBuffer);

	// проверить, что размер правильный
	if(size > uniformBuffer->GetSize())
		THROW_PRIMARY_EXCEPTION("Uniform buffer data size too big");

	// http://www.opengl.org/wiki/GLAPI/glBindBuffer
	// Сказано, что такая привязка не снимает буферы, привязанные
	// через glBindBufferBase. Верим, но надо проверить.
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->GetName());
	void* bufferData = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	if(!bufferData)
	{
		GlSystem::CheckErrors("Can't map uniform buffer");
		return;
	}

	memcpy(bufferData, data, size);

	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void GlContext::Draw()
{
	Update();

	glDrawArrays(GL_TRIANGLES, 0, boundIndexBuffer ? boundIndexBuffer->GetIndicesCount() : boundVertexBuffer->GetVerticesCount());
}

void GlContext::DrawInstanced(int instancesCount)
{
	Update();

	glDrawArraysInstanced(GL_TRIANGLES, 0, boundIndexBuffer ? boundIndexBuffer->GetIndicesCount() : boundVertexBuffer->GetVerticesCount(), instancesCount);
}
