#include "GlContext.hpp"
#include "GlSystem.hpp"
#include "GlInternalProgramCache.hpp"
#include "GlInternalProgram.hpp"
#include "GlInternalAttributeBinding.hpp"
#include "GlInternalAttributeBindingCache.hpp"
#include "GlRenderBuffer.hpp"
#include "GlDepthStencilBuffer.hpp"
#include "GlTexture.hpp"
#include "GlSamplerState.hpp"
#include "GlUniformBuffer.hpp"
#include "GlVertexShader.hpp"
#include "GlPixelShader.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlBlendState.hpp"
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
	if(targetState.renderBuffers[0] && fast_cast<GlRenderBuffer*>(&*targetState.renderBuffers[0])->GetName() == 0)
	{
		if(targetState.depthStencilBuffer)
			THROW_PRIMARY_EXCEPTION("Default renderbuffer can be bound only without depth-stencil buffer");
		for(int i = 1; i < ContextState::renderTargetSlotsCount; ++i)
			if(targetState.renderBuffers[i])
				THROW_PRIMARY_EXCEPTION("Default renderbuffer can be bound only without other buffers");

		// привязать фреймбуфер по умолчанию
		if(currentFramebuffer != 0)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer = 0);

		// больше ничего привязывать не надо

		// установить, что всё привязано
		for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
			boundState.renderBuffers[i] = targetState.renderBuffers[i];
		boundState.depthStencilBuffer = targetState.depthStencilBuffer;
	}
	// иначе все рендербуферы - не по умолчанию
	// определяем, что поменялось, и меняем
	else
	{
		// привязать нужный фреймбуфер
		if(currentFramebuffer != targetsFramebuffer)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer = targetsFramebuffer);

		// привязать изменившиеся рендербуферы
		for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
			if(targetState.renderBuffers[i] != boundState.renderBuffers[i])
			{
				RenderBuffer* abstractRenderBuffer = targetState.renderBuffers[i];
				if(abstractRenderBuffer)
				{
					GLuint name = fast_cast<GlRenderBuffer*>(abstractRenderBuffer)->GetName();
					if(name == 0)
						THROW_PRIMARY_EXCEPTION("Default renderbuffer can't be bound with other buffers");
					glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, name, 0);
				}
				else
					glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 0, 0);

				boundState.renderBuffers[i] = targetState.renderBuffers[i];
			}

		// привязать изменившийся depth-stencil буфер
		if(targetState.depthStencilBuffer != boundState.depthStencilBuffer)
		{
			DepthStencilBuffer* abstractDepthStencilBuffer = targetState.depthStencilBuffer;
			glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				abstractDepthStencilBuffer ? fast_cast<GlDepthStencilBuffer*>(abstractDepthStencilBuffer)->GetName() : 0, 0);

			boundState.depthStencilBuffer = targetState.depthStencilBuffer;
		}
	}

	// текстуры
	for(int i = 0; i < ContextState::textureSlotsCount; ++i)
		if(targetState.textures[i] != boundState.textures[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			Texture* abstractTexture = targetState.textures[i];
			glBindTexture(GL_TEXTURE_2D, abstractTexture ? fast_cast<GlTexture*>(abstractTexture)->GetName() : 0);

			boundState.textures[i] = targetState.textures[i];
		}
	// семплеры
	for(int i = 0; i < ContextState::textureSlotsCount; ++i)
		if(targetState.samplerStates[i] != boundState.samplerStates[i])
		{
			SamplerState* abstractSamplerState = targetState.samplerStates[i];
			glBindSampler(i, abstractSamplerState ? fast_cast<GlSamplerState*>(abstractSamplerState)->GetName() : 0);

			boundState.samplerStates[i] = targetState.samplerStates[i];
		}

	// uniform-буферы
	for(int i = 0; i < ContextState::uniformBufferSlotsCount; ++i)
		if(targetState.uniformBuffers[i] != boundState.uniformBuffers[i])
		{
			UniformBuffer* abstractUniformBuffer = targetState.uniformBuffers[i];
			glBindBufferBase(GL_UNIFORM_BUFFER, i, abstractUniformBuffer ? fast_cast<GlUniformBuffer*>(abstractUniformBuffer)->GetName() : 0);

			boundState.uniformBuffers[i] = targetState.uniformBuffers[i];
		}

	bool dirtyAttributeBinding = false;

	// вершинный и пиксельный шейдеры
	if(targetState.vertexShader != boundState.vertexShader || targetState.pixelShader != boundState.pixelShader)
	{
		ptr<GlInternalProgram> program = programCache->GetProgram(fast_cast<GlVertexShader*>(&*targetState.vertexShader), fast_cast<GlPixelShader*>(&*targetState.pixelShader));
		if(boundProgram != program)
		{
			glUseProgram(boundProgram->GetName());
			boundProgram = program;
		}

		boundState.vertexShader = targetState.vertexShader;
		boundState.pixelShader = targetState.pixelShader;
		dirtyAttributeBinding = true;
	}

	// вершинный буфер
	if(targetState.vertexBuffer != boundState.vertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, fast_cast<GlVertexBuffer*>(&*targetState.vertexBuffer)->GetName());

		boundState.vertexBuffer = targetState.vertexBuffer;
		dirtyAttributeBinding = true;
	}

	// индексный буфер
	if(targetState.indexBuffer != boundState.indexBuffer)
	{
		IndexBuffer* abstractIndexBuffer = targetState.indexBuffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, abstractIndexBuffer ? fast_cast<GlVertexBuffer*>(abstractIndexBuffer)->GetName() : 0);

		boundState.indexBuffer = targetState.indexBuffer;
		dirtyAttributeBinding = true;
	}

	// привязка к атрибутам
	if(dirtyAttributeBinding)
	{
		ptr<Layout> vertexLayout = targetState.vertexBuffer->GetLayout();
		ptr<GlInternalAttributeBinding> attributeBinding = attributeBindingCache->GetBinding(vertexLayout, boundProgram);
		if(boundAttributeBinding != attributeBinding)
		{
			// выполнить привязку
			int stride = vertexLayout->GetStride();
			const std::vector<GlInternalAttributeBinding::Element>& elements = boundAttributeBinding->GetElements();
			for(size_t i = 0; i < elements.size(); ++i)
			{
				const GlInternalAttributeBinding::Element& element = elements[i];

				glVertexAttribPointer(element.index, element.size, element.type, element.normalized, stride, element.pointer);
			}

			boundAttributeBinding = attributeBinding;
		}

		dirtyAttributeBinding = false;
	}

	if(targetState.fillMode != boundState.fillMode)
	{
		switch(targetState.fillMode)
		{
		case ContextState::fillModeWireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case ContextState::fillModeSolid:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			THROW_PRIMARY_EXCEPTION("Unknown fill mode");
		}

		boundState.fillMode = targetState.fillMode;
	}

	if(targetState.cullMode != boundState.cullMode)
	{
		switch(targetState.cullMode)
		{
		case ContextState::cullModeNone:
			glDisable(GL_CULL_FACE);
			break;
		case ContextState::cullModeBack:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case ContextState::cullModeFront:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		default:
			THROW_PRIMARY_EXCEPTION("Unknown cull mode");
		}

		boundState.cullMode = targetState.cullMode;
	}

	if(targetState.viewportWidth != boundState.viewportWidth || targetState.viewportHeight != boundState.viewportHeight)
	{
		glViewport(0, 0, targetState.viewportWidth, targetState.viewportHeight);

		boundState.viewportWidth = targetState.viewportWidth;
		boundState.viewportHeight = targetState.viewportHeight;
	}

	if(targetState.depthTestFunc != boundState.depthTestFunc)
	{
		GLenum func;
		switch(targetState.depthTestFunc)
		{
		case ContextState::depthTestFuncNever:
			func = GL_NEVER;
			break;
		case ContextState::depthTestFuncLess:
			func = GL_LESS;
			break;
		case ContextState::depthTestFuncLessOrEqual:
			func = GL_LEQUAL;
			break;
		case ContextState::depthTestFuncEqual:
			func = GL_EQUAL;
			break;
		case ContextState::depthTestFuncNonEqual:
			func = GL_NOTEQUAL;
			break;
		case ContextState::depthTestFuncGreaterOrEqual:
			func = GL_GEQUAL;
			break;
		case ContextState::depthTestFuncGreater:
			func = GL_GREATER;
			break;
		case ContextState::depthTestFuncAlways:
			func = GL_ALWAYS;
			break;
		default:
			THROW_PRIMARY_EXCEPTION("Unknown depth test func");
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(func);

		boundState.depthTestFunc = targetState.depthTestFunc;
	}

	if(targetState.depthWrite != boundState.depthWrite)
	{
		glDepthMask(targetState.depthWrite ? GL_TRUE : GL_FALSE);

		boundState.depthWrite = targetState.depthWrite;
	}

	if(targetState.blendState != boundState.blendState)
	{
		if(targetState.blendState)
			fast_cast<GlBlendState*>(&*targetState.blendState)->Apply();

		boundState.blendState = targetState.blendState;
	}

	GlSystem::CheckErrors("Can't update context");
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

	glDrawArrays(GL_TRIANGLES, 0, boundState.indexBuffer ? boundState.indexBuffer->GetIndicesCount() : boundState.vertexBuffer->GetVerticesCount());
}

void GlContext::DrawInstanced(int instancesCount)
{
	Update();

	glDrawArraysInstanced(GL_TRIANGLES, 0, boundState.indexBuffer ? boundState.indexBuffer->GetIndicesCount() : boundState.vertexBuffer->GetVerticesCount(), instancesCount);
}
